#pragma once

#include "../Resource.h"
#include <memory>
#include <sol/sol.hpp>
#include <Libs/Utilities/TransparentTypes.h>

namespace tactics::resource {

class ResourceManager;
class ResourcePathHelper;

using ResourceManagerProvider = std::function<ResourceManager* (ResourceType)>;

class ResourcePackManager {
public:
	ResourcePackManager(sol::state_view luaState, const ResourcePathHelper& pathHelper, const ResourceManagerProvider& managerProvider);
	void loadPackDefinition(std::string_view packDefinitionPath);

	void loadPack(std::string_view packName);
	void unloadPack(std::string_view packName);
	void unloadAllPacks();

private:
	struct PackageLoadInfo {
		ResourceType resourceType;
		sol::reference luaFunction;
	};

	struct Package {
		std::string name;
		std::vector<std::unique_ptr<PackageLoadInfo>> loadInfos;
		std::unordered_map<ResourceType, std::vector<ResourceId>> loadedResources;
	};

	Package& _getResourcePack(std::string_view packName);
	void _loadPack(Package& package, PackageLoadInfo& loadInfo);
	void _unloadPack(Package& package);
	void _addLoadedResources(Package& package, ResourceType resourceType, std::vector<ResourceId> resourceIds);

	const ResourcePathHelper& _pathHelper;
	sol::state_view _luaState;
	UnorderedStringMap<std::unique_ptr<Package>> _packages;
	ResourceManagerProvider _managerProvider;
};

}
