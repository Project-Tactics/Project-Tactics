#include "Test_Fixture.h"
#include <Libs/Utility/Exception.h>

TEST_F(ResourceTest, EmptySystemWithNoResources) {
	EXPECT_EQ(getNumberOfResources(), 0);
}

TEST_F(ResourceTest, GetResourceWithoutManager) {
	EXPECT_THROW([[maybe_unused]] auto res = _resourceSystem->getResource<MockTextureResource>("MyResourceName1"), Exception);
}

TEST_F(ResourceTest, KeepReferenceToResource) {
	givenValidPackDefinitionLoaded();
	givenValidManagerRegistered();

	_resourceSystem->createManualPack("customPackage");
	_resourceSystem->loadExternalResource<MockShaderResource>("customPackage", "MyResourceName1", {{"testResource", "testResourceData"}});
	auto referencedResource = _resourceSystem->getResource<MockShaderResource>("MyResourceName1");
	EXPECT_THROW(_resourceSystem->unloadPack("customPackage"), Exception);
}

TEST_F(ResourceTest, RemoveReferenceBeforeUnloadingPack) {
	givenValidPackDefinitionLoaded();
	givenValidManagerRegistered();

	_resourceSystem->createManualPack("customPackage");
	_resourceSystem->loadExternalResource<MockShaderResource>("customPackage", "MyResourceName1", {{"testResource", "testResourceData"}});
	auto referencedResource = _resourceSystem->getResource<MockShaderResource>("MyResourceName1");
	referencedResource.reset();
	_resourceSystem->unloadPack("customPackage");
}
