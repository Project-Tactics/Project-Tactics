#pragma once

#include "../Resource.h"

#include <Libs/Utility/TransparentTypes.h>

#include <nlohmann/json.hpp>
#include <memory>
#include <functional>

namespace tactics::resource {

class BaseResourceManager;
class ResourcePathHelper;

using ResourceManagerProvider = std::function<BaseResourceManager& (ResourceType)>;

class ResourcePackManager {
public:
	ResourcePackManager(const ResourcePathHelper& pathHelper, const ResourceManagerProvider& managerProvider);
	void loadPackDefinition(std::string_view packDefinitionPath);

	void loadPack(std::string_view packName);
	void unloadPack(std::string_view packName);
	void unloadAllPacks();
	void createPack(std::string_view packName);
	void registerResource(std::string_view packName, std::shared_ptr<BaseResource> resource);
	void loadResource(std::string_view packName, const nlohmann::json& descriptor, ResourceType type);

private:
	struct PackGroup {
		ResourceType type;
		std::vector<nlohmann::ordered_json> descriptors;
		std::vector<ResourceId> loadedResources;
	};

	struct Pack {
		std::string name;
		std::unordered_map<ResourceType, std::unique_ptr<PackGroup>> groups;

		PackGroup& getOrCreatePackGroup(ResourceType type);
	};

	Pack& _getResourcePack(std::string_view packName);
	void _loadPack(Pack& pack);
	void _unloadPack(Pack& pack);

	const ResourcePathHelper& _pathHelper;
	UnorderedStringMap<std::unique_ptr<Pack>> _packs;
	ResourceManagerProvider _managerProvider;
};

}
