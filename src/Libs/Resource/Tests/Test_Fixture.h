#include <Libs/FileSystem/FileLoader.h>
#include <Libs/FileSystem/FileSystem.h>
#include <Libs/Resource/ResourceSystem.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace tactics;
using namespace tactics::resource;

using ::testing::_;
using ::testing::AtLeast;
using ::testing::ByMove;
using ::testing::Exactly;
using ::testing::Invoke;
using ::testing::Return;

class MockShaderResource : public Resource<MockShaderResource> {
public:
	using Resource::Resource;
	static constexpr ResourceType TYPE = ResourceType::Shader;
};

class MockTextureResource : public Resource<MockTextureResource> {
public:
	using Resource::Resource;
	static constexpr ResourceType TYPE = ResourceType::Texture;
};

class MockShaderLoader : public ResourceLoader {
public:
	using ResourceLoader::ResourceLoader;
	MOCK_METHOD(std::shared_ptr<MockShaderResource>, load, (const HashId&, const nlohmann::ordered_json&));
};

class MockTextureLoader : public ResourceLoader {
public:
	using ResourceLoader::ResourceLoader;

	std::shared_ptr<MockTextureResource> load(const nlohmann::ordered_json&) { return nullptr; }
};

using MockShaderManager = ResourceManager<MockShaderResource, MockShaderLoader>;
using MockTextureManager = ResourceManager<MockTextureResource, MockTextureLoader>;

class MockFileLoader : public FileLoader {
public:
	MOCK_METHOD(std::unique_ptr<FileHandle<std::string>>,
				createStringFile,
				(const std::filesystem::path&),
				(const override));
	MOCK_METHOD(std::unique_ptr<FileHandle<nlohmann::ordered_json>>,
				createJsonFile,
				(const std::filesystem::path&),
				(const override));
	MOCK_METHOD(std::unique_ptr<FileHandle<ini::IniFile>>, createIni, (const std::filesystem::path&), (const override));
};

class MockJsonFileHandle : public FileHandle<nlohmann::ordered_json> {
public:
	MOCK_METHOD(bool, exists, (), (const override));
	MOCK_METHOD(void, save, (), (override));
	MOCK_METHOD(void, load, (), (override));

	void setData(const nlohmann::ordered_json& data) { _setContent(data); }
};

class MockResourceProvider : public ResourceProvider {
public:
	MOCK_METHOD(std::shared_ptr<BaseResource>, getResource, (ResourceType, const HashId&), (const override));
	MOCK_METHOD(std::shared_ptr<BaseResource>, getResource, (ResourceType, ResourceId), (const override));
	MOCK_METHOD(BaseResourceManager&, getManager, (ResourceType), (const override));
	MOCK_METHOD(BaseResourceManager&, getManager, (ResourceType), (override));
};

class ResourceTest : public testing::Test {
public:
	void SetUp() {
		auto fileLoader = std::make_unique<MockFileLoader>();
		_fileLoader = fileLoader.get();
		_fileSystem = std::make_unique<FileSystem>(std::move(fileLoader), std::make_unique<PathHelper>(""));
		_resourceSystem = std::make_unique<ResourceSystem>(*_fileSystem);
	}

	void expectJsonFileLoad(int times) { EXPECT_CALL(*_jsonPackFileHandle, load()).Times(Exactly(times)); }

	void givenJsonFileCreation() {
		_jsonPackFileHandleUniquePtr = std::make_unique<MockJsonFileHandle>();
		_jsonPackFileHandle = _jsonPackFileHandleUniquePtr.get();

		EXPECT_CALL(*_fileLoader, createJsonFile(_)).WillOnce(Return(ByMove(std::move(_jsonPackFileHandleUniquePtr))));
	}

	void givenPackDefinitionExists(bool exists) {
		EXPECT_CALL(*_jsonPackFileHandle, exists()).WillOnce(Return(exists));
	}

	void givenDefaultPackDefinition() {
		_jsonPackFileHandle->setData({{"mainPackage",
									   {{"shader",
										 {{"MyResourceName1", {{"testResource", "testResourceData"}}},
										  {"MyResourceName2", {{"testResource", "testResourceData"}}},
										  {"MyResourceName3", {{"testResource", "testResourceData"}}}}}}}});
	}

	void givenValidPackDefinitionLoaded() {
		givenJsonFileCreation();
		givenDefaultPackDefinition();
		givenPackDefinitionExists(true);
		expectJsonFileLoad(1);

		_resourceSystem->loadPackDefinition("TestPackDefinition.json");
	}

	void givenValidManagerRegistered() {
		auto loader = std::make_unique<MockShaderLoader>(*_fileSystem, *_resourceSystem);
		EXPECT_CALL(*loader, load(_, _)).WillRepeatedly([](const HashId& name, const nlohmann::ordered_json&) {
			auto res = std::make_shared<MockShaderResource>(name);
			return res;
		});
		auto manager = std::make_unique<MockShaderManager>(std::move(loader));
		_resourceSystem->registerManager(std::move(manager));
	}

	int getNumberOfResources() {
		int count = 0;
		_resourceSystem->forEachResource([&count](const auto&, const auto&, const auto&) { ++count; });
		return count;
	}

	int getNumberOfLoadedResources() {
		int countFromPacks = 0;
		_resourceSystem->forEachResource([&countFromPacks](const auto& pack, const auto&, const auto&) {
			if (pack.isLoaded()) { ++countFromPacks; }
		});
		int countFromManagers = 0;
		_resourceSystem->forEachManager([&countFromManagers](const auto& manager) {
			manager.forEachResource([&countFromManagers](const auto&) { ++countFromManagers; });
		});
		EXPECT_EQ(countFromPacks, countFromManagers);
		return countFromPacks;
	}

	int getNumberOfPacks() {
		int count = 0;
		_resourceSystem->forEachPack([&count](const Pack&) { ++count; });
		return count;
	}

protected:
	std::unique_ptr<ResourceSystem> _resourceSystem;
	std::unique_ptr<FileSystem> _fileSystem;
	std::unique_ptr<MockJsonFileHandle> _jsonPackFileHandleUniquePtr;
	MockJsonFileHandle* _jsonPackFileHandle;
	MockFileLoader* _fileLoader;
};
