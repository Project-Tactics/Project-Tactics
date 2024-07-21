#include "Test_Fixture.h"

#include <Libs/Utility/Exception.h>

TEST_F(ResourceTest, LoadInvalidPack) {
	EXPECT_THROW(_resourceSystem->loadPack("InvalidPack"_id), Exception);
}

TEST_F(ResourceTest, LoadValidPackWithMissingManager) {
	givenValidPackDefinitionLoaded();

	EXPECT_THROW(_resourceSystem->loadPack("mainPackage"_id), Exception);
}

TEST_F(ResourceTest, LoadValidPackWithOnlyOneWrongManager) {
	givenValidPackDefinitionLoaded();

	auto loader = std::make_unique<MockTextureLoader>(*_fileSystem, *_resourceSystem);
	auto manager = std::make_unique<MockTextureManager>(std::move(loader));
	_resourceSystem->registerManager(std::move(manager));
	EXPECT_THROW(_resourceSystem->loadPack("mainPackage"_id), Exception);
}

TEST_F(ResourceTest, LoadValidPack) {
	givenValidPackDefinitionLoaded();

	auto loader = std::make_unique<MockShaderLoader>(*_fileSystem, *_resourceSystem);
	EXPECT_CALL(*loader, load(_, _)).WillRepeatedly([](const HashId& name, const nlohmann::ordered_json&) {
		return std::make_shared<MockShaderResource>(name);
	});
	auto manager = std::make_unique<MockShaderManager>(std::move(loader));

	_resourceSystem->registerManager(std::move(manager));

	_resourceSystem->loadPack("mainPackage"_id);
}

TEST_F(ResourceTest, LoadValidPackAndGetResources) {
	givenValidPackDefinitionLoaded();

	auto loader = std::make_unique<MockShaderLoader>(*_fileSystem, *_resourceSystem);
	EXPECT_CALL(*loader, load(_, _)).WillRepeatedly([](const HashId& name, const nlohmann::ordered_json&) {
		return std::make_shared<MockShaderResource>(name);
	});
	auto manager = std::make_unique<MockShaderManager>(std::move(loader));

	_resourceSystem->registerManager(std::move(manager));

	_resourceSystem->loadPack("mainPackage"_id);

	auto res1 = _resourceSystem->getResource<MockShaderResource>("MyResourceName1"_id);
	auto res2 = _resourceSystem->getResource<MockShaderResource>("MyResourceName2"_id);
	auto res3 = _resourceSystem->getResource<MockShaderResource>("MyResourceName3"_id);
}

TEST_F(ResourceTest, LoadValidPackAndGetResourceWithWrongName) {
	givenValidPackDefinitionLoaded();

	auto loader = std::make_unique<MockShaderLoader>(*_fileSystem, *_resourceSystem);
	EXPECT_CALL(*loader, load(_, _)).WillRepeatedly([](const HashId& name, const nlohmann::ordered_json&) {
		return std::make_shared<MockShaderResource>(name);
	});
	auto manager = std::make_unique<MockShaderManager>(std::move(loader));

	_resourceSystem->registerManager(std::move(manager));

	_resourceSystem->loadPack("mainPackage"_id);

	EXPECT_THROW([[maybe_unused]] auto res = _resourceSystem->getResource<MockShaderResource>("MyResource"_id),
				 Exception);
}

TEST_F(ResourceTest, LoadPackTwice) {
	givenValidPackDefinitionLoaded();
	givenValidManagerRegistered();

	_resourceSystem->loadPack("mainPackage"_id);

	EXPECT_THROW(_resourceSystem->loadPack("mainPackage"_id), Exception);
}

TEST_F(ResourceTest, ReloadPackAfterUnloading) {
	givenValidPackDefinitionLoaded();
	givenValidManagerRegistered();

	for (auto i = 0; i < 10; ++i) {
		_resourceSystem->loadPack("mainPackage"_id);
		_resourceSystem->unloadPack("mainPackage"_id);
	}
	_resourceSystem->loadPack("mainPackage"_id);
}

TEST_F(ResourceTest, NoResourceLeftAfterUnloadingPack) {
	givenValidPackDefinitionLoaded();
	givenValidManagerRegistered();

	for (auto i = 0; i < 10; ++i) {
		_resourceSystem->loadPack("mainPackage"_id);
		_resourceSystem->unloadPack("mainPackage"_id);
	}

	EXPECT_EQ(getNumberOfLoadedResources(), 0);
}

TEST_F(ResourceTest, CorrectResourcesLeftAfterReloading) {
	givenValidPackDefinitionLoaded();
	givenValidManagerRegistered();

	for (auto i = 0; i < 10; ++i) {
		_resourceSystem->loadPack("mainPackage"_id);
		_resourceSystem->unloadPack("mainPackage"_id);
	}
	_resourceSystem->loadPack("mainPackage"_id);

	EXPECT_EQ(getNumberOfLoadedResources(), 3);
}

TEST_F(ResourceTest, UnloadPackWhichHasNotBeenLoaded) {
	givenValidPackDefinitionLoaded();
	givenValidManagerRegistered();

	EXPECT_THROW(_resourceSystem->unloadPack("mainPackage"_id), Exception);
}

TEST_F(ResourceTest, CreateCustomPack) {
	givenValidManagerRegistered();

	_resourceSystem->createManualPack("customPackage"_id);
	EXPECT_EQ(getNumberOfPacks(), 1);
}

TEST_F(ResourceTest, CreateExtraCustomPack) {
	givenValidPackDefinitionLoaded();
	givenValidManagerRegistered();

	_resourceSystem->createManualPack("customPackage"_id);
	EXPECT_EQ(getNumberOfPacks(), 2);
}

TEST_F(ResourceTest, CreateCustomPackWithSameName) {
	givenValidPackDefinitionLoaded();
	givenValidManagerRegistered();

	_resourceSystem->createManualPack("customPackage"_id);
	EXPECT_THROW(_resourceSystem->createManualPack("customPackage"_id), Exception);
}

TEST_F(ResourceTest, LoadCustomPack) {
	givenValidPackDefinitionLoaded();
	givenValidManagerRegistered();

	_resourceSystem->createManualPack("customPackage"_id);
	EXPECT_THROW(_resourceSystem->loadPack("customPackage"_id), Exception);
}

TEST_F(ResourceTest, UnloadCustomPack) {
	givenValidPackDefinitionLoaded();
	givenValidManagerRegistered();

	_resourceSystem->createManualPack("customPackage"_id);
	_resourceSystem->unloadPack("customPackage"_id);
}

TEST_F(ResourceTest, ReloadCustomPack) {
	givenValidPackDefinitionLoaded();
	givenValidManagerRegistered();

	_resourceSystem->createManualPack("customPackage"_id);
	_resourceSystem->unloadPack("customPackage"_id);
	EXPECT_THROW(_resourceSystem->loadPack("customPackage"_id), Exception);
}

TEST_F(ResourceTest, RegisterExternalResource) {
	givenValidPackDefinitionLoaded();
	givenValidManagerRegistered();

	_resourceSystem->createManualPack("customPackage"_id);
	_resourceSystem->loadExternalResource("customPackage"_id,
										  std::make_shared<MockShaderResource>("MyResourceName1"_id));

	EXPECT_EQ(getNumberOfLoadedResources(), 1);
}

TEST_F(ResourceTest, RegisterExternalResourceWithJson) {
	givenValidPackDefinitionLoaded();
	givenValidManagerRegistered();

	_resourceSystem->createManualPack("customPackage"_id);
	nlohmann::ordered_json json = {{"testResource", "testResourceData"}};
	_resourceSystem->loadExternalResource<MockShaderResource>("customPackage"_id, "MyResourceName1"_id, json);

	EXPECT_EQ(getNumberOfLoadedResources(), 1);
	[[maybe_unused]] auto res = _resourceSystem->getResource<MockShaderResource>("MyResourceName1"_id);
}

TEST_F(ResourceTest, RegisterExternalResourceWithSameName) {
	givenValidPackDefinitionLoaded();
	givenValidManagerRegistered();

	_resourceSystem->createManualPack("customPackage"_id);
	_resourceSystem->createManualPack("anotherPackage"_id);
	nlohmann::ordered_json json = {{"testResource", "testResourceData"}};
	_resourceSystem->loadExternalResource<MockShaderResource>("customPackage"_id, "MyResourceName1"_id, json);
	EXPECT_THROW(
		_resourceSystem->loadExternalResource<MockShaderResource>("anotherPackage"_id, "MyResourceName1"_id, json),
		Exception);
}

TEST_F(ResourceTest, RegisterExternalResourceCleanPackAndRegisterAgain) {
	givenValidPackDefinitionLoaded();
	givenValidManagerRegistered();

	_resourceSystem->createManualPack("customPackage"_id);
	nlohmann::ordered_json json = {{"testResource", "testResourceData"}};
	_resourceSystem->loadExternalResource<MockShaderResource>("customPackage"_id, "MyResourceName1"_id, json);
	_resourceSystem->unloadPack("customPackage"_id);
	_resourceSystem->loadExternalResource<MockShaderResource>("customPackage"_id, "MyResourceName1"_id, json);
}

TEST_F(ResourceTest, CountResourcesAndLoadedResources) {
	givenValidPackDefinitionLoaded();
	givenValidManagerRegistered();

	_resourceSystem->createManualPack("customPackage"_id);
	nlohmann::ordered_json json = {{"testResource", "testResourceData"}};
	_resourceSystem->loadExternalResource<MockShaderResource>("customPackage"_id, "MyResourceName1"_id, json);

	EXPECT_EQ(getNumberOfResources(), 4);
	EXPECT_EQ(getNumberOfLoadedResources(), 1);
}
