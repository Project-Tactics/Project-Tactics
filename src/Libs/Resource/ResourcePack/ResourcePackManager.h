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

	void loadPack(const HashId& packName);
	void unloadPack(const HashId& packName);
	Pack& createPack(const HashId& packName, bool manuallyCreated);

	void loadExternalResource(const HashId& packName, std::shared_ptr<BaseResource> resource);
	void loadExternalResource(const HashId& packName, const HashId& resourceName, ResourceType type, const nlohmann::json& data);

	void forEachPack(const std::function<void(const Pack&)>& callback);

private:
	[[nodiscard]] Pack& _getResourcePack(const HashId& packName);
	[[nodiscard]] std::unique_ptr<FileHandle<nlohmann::ordered_json>> _loadPackDefinition(const std::filesystem::path& packDefinitionPath);

	FileSystem& _fileSystem;
	const ResourceProvider& _resourceProvider;
	std::unordered_map<HashId, std::unique_ptr<Pack>> _packs;
};

}
