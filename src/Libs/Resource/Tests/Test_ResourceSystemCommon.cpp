#include "Test_Fixture.h"
#include <Libs/Utility/Exception.h>

TEST_F(ResourceTest, EmptySystemWithNoResources) {
	EXPECT_EQ(getNumberOfResources(), 0);
}

TEST_F(ResourceTest, GetResourceWithoutManager) {
	EXPECT_THROW([[maybe_unused]] auto res = _resourceSystem->getResource<MockTextureResource>("MyResourceName1"_id), Exception);
}

TEST_F(ResourceTest, KeepReferenceToResource) {
	givenValidPackDefinitionLoaded();
	givenValidManagerRegistered();

	_resourceSystem->createManualPack("customPackage"_id);
	_resourceSystem->loadExternalResource<MockShaderResource>("customPackage"_id, "MyResourceName1"_id, {{"testResource", "testResourceData"}});
	auto referencedResource = _resourceSystem->getResource<MockShaderResource>("MyResourceName1"_id);
	EXPECT_THROW(_resourceSystem->unloadPack("customPackage"_id), Exception);
}

TEST_F(ResourceTest, RemoveReferenceBeforeUnloadingPack) {
	givenValidPackDefinitionLoaded();
	givenValidManagerRegistered();

	_resourceSystem->createManualPack("customPackage"_id);
	_resourceSystem->loadExternalResource<MockShaderResource>("customPackage"_id, "MyResourceName1"_id, {{"testResource", "testResourceData"}});
	auto referencedResource = _resourceSystem->getResource<MockShaderResource>("MyResourceName1"_id);
	referencedResource.reset();
	_resourceSystem->unloadPack("customPackage"_id);
}
