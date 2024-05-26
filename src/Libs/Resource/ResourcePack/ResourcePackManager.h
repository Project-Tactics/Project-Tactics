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

private:
	struct PackGroup {
		ResourceType type;
		std::vector<nlohmann::json> descriptors;
		std::vector<ResourceId> loadedResources;
	};

	struct Pack {
		std::string name;
		std::unordered_map<ResourceType, std::unique_ptr<PackGroup>> groups;
	};

	Pack& _getResourcePack(std::string_view packName);
	void _loadPack(Pack& pack);
	void _unloadPack(Pack& pack);

	const ResourcePathHelper& _pathHelper;
	UnorderedStringMap<std::unique_ptr<Pack>> _packs;
	ResourceManagerProvider _managerProvider;
};

}
