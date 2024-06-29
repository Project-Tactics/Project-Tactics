#pragma once

#include "../Resource.h"

#include "ResourcePack.h"

#include <Libs/FileSystem/FileSystem.h>
#include <Libs/Utility/TransparentTypes.h>

#include <nlohmann/json.hpp>
#include <memory>
#include <functional>

namespace tactics {
class FileSystem;
}

namespace tactics::resource {

class BaseResourceManager;

class ResourcePackManager {
public:
	ResourcePackManager(FileSystem& pathHelper, const ResourceProvider& resourceProvider);
	void loadPackDefinition(const std::filesystem::path& packDefinitionPath);

	void loadPack(const hash_string& packName);
	void unloadPack(const hash_string& packName);
	Pack& createPack(const hash_string& packName, bool manuallyCreated);

	void loadExternalResource(const hash_string& packName, std::shared_ptr<BaseResource> resource);
	void loadExternalResource(const hash_string& packName, const hash_string& resourceName, ResourceType type, const nlohmann::json& data);

	void forEachPack(const std::function<void(const Pack&)>& callback);

private:
	[[nodiscard]] Pack& _getResourcePack(const hash_string& packName);
	[[nodiscard]] std::unique_ptr<FileHandle<nlohmann::ordered_json>> _loadPackDefinition(const std::filesystem::path& packDefinitionPath);

	FileSystem& _fileSystem;
	const ResourceProvider& _resourceProvider;
	std::unordered_map<hash_string::hash_type, std::unique_ptr<Pack>> _packs;
};

}
