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

	void loadPack(std::string_view packName);
	void unloadPack(std::string_view packName);
	void unloadAllPacks();
	Pack& createPack(std::string_view packName, bool manuallyCreated);

	void loadExternalResource(std::string_view packName, std::shared_ptr<BaseResource> resource);
	void loadExternalResource(std::string_view packName, std::string_view resourceName, ResourceType type, const nlohmann::json& data);

	void forEachPack(const std::function<void(const Pack&)>& callback);

private:
	[[nodiscard]] Pack& _getResourcePack(std::string_view packName);
	[[nodiscard]] std::unique_ptr<FileHandle<nlohmann::ordered_json>> _loadPackDefinition(const std::filesystem::path& packDefinitionPath);

	FileSystem& _fileSystem;
	const ResourceProvider& _resourceProvider;
	UnorderedStringMap<std::unique_ptr<Pack>> _packs;
};

}
