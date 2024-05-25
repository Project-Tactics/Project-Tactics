#include "ResourcePackManager.h"

#include "../ResourcePathHelper.h"
#include "../ResourceManager.h"

#include <Libs/Utilities/Exception.h>

namespace tactics::resource {

ResourcePackManager::ResourcePackManager(sol::state_view luaState, const ResourcePathHelper& pathHelper, const ResourceManagerProvider& managerProvider)
	: _luaState(luaState)
	, _pathHelper(pathHelper)
	, _managerProvider(managerProvider) {
}

void ResourcePackManager::loadPackDefinition(std::string_view packDefinitionPath) {
	sol::environment resourcePackEnv = _luaState["__resourceEnvTable"];
	resourcePackEnv.set_function("resourcePack", [this] (sol::table packageTable) {
		auto package = std::make_unique<Package>();

		for (auto&& [keyObj, value] : packageTable) {
			std::string_view key = keyObj.as<std::string_view>();
			if (key == "name") {
				package->name = value.as<std::string_view>();
				if (_packages.contains(package->name)) {
					throw Exception("Can't add package {}. A package with the same name already exists.", package->name);
				}
			} else {
				auto resourceType = ResourceTypeSerialization::fromGroupTypeToEnum(key);
				auto function = value.as<sol::function>();
				if (function.valid()) {
					auto functionReference = sol::make_reference(_luaState, function);
					auto loadInfo = std::make_unique<PackageLoadInfo>();
					loadInfo->resourceType = resourceType;
					loadInfo->luaFunction = functionReference;
					package->loadInfos.push_back(std::move(loadInfo));
				} else {
					throw Exception("Can't add package {}. A lua function has not been defined for Resource type {}",
						package->name, key);
				}
			}
		}
		_packages.insert({package->name, std::move(package)});
	});

	auto path = _pathHelper.makeAbsolutePath(packDefinitionPath);
	_luaState.script_file(path, resourcePackEnv);

	resourcePackEnv["resourcePack"] = sol::nil;
}

void ResourcePackManager::loadPack(std::string_view packName) {
	auto& resourcePack = _getResourcePack(packName);
	for (auto&& loader : resourcePack.loadInfos) {
		_loadPack(resourcePack, *loader);
	}
}

void ResourcePackManager::_loadPack(Package& package, PackageLoadInfo& loadInfo) {
	auto manager = _managerProvider(loadInfo.resourceType);
	auto resourceIds = manager->load(loadInfo.luaFunction);
	if (resourceIds.empty()) {
		throw Exception("A section for resource type \"{}\" has been defined in package \"{}\" but no resources have been loaded.",
			ResourceTypeSerialization::toString(loadInfo.resourceType), package.name);
	}
	_addLoadedResources(package, loadInfo.resourceType, resourceIds);
}

void ResourcePackManager::_addLoadedResources(Package& package, ResourceType resourceType, std::vector<ResourceId> resourceIds) {
	for (auto& resourceId : resourceIds) {
		if (auto itr = package.loadedResources.find(resourceType); itr == package.loadedResources.end()) {
			package.loadedResources[resourceType] = {resourceId};
		} else {
			itr->second.push_back(resourceId);
		}
	}
}

void ResourcePackManager::unloadPack(std::string_view packName) {
	auto& package = _getResourcePack(packName);
	_unloadPack(package);
}

void ResourcePackManager::_unloadPack(Package& package) {
	for (auto&& [resourceType, loadedResources] : package.loadedResources) {
		auto manager = _managerProvider(resourceType);
		for (auto&& resourceId : loadedResources) {
			manager->unload(resourceId);
		}
		loadedResources.clear();
	}
}

void ResourcePackManager::unloadAllPacks() {
	for (auto&& [packName, package] : _packages) {
		_unloadPack(*package);
	}
}

ResourcePackManager::Package& ResourcePackManager::_getResourcePack(std::string_view packName) {
	if (!_packages.contains(packName)) {
		throw Exception("Can't find resource pack with name {}. The Resource Pack does not exists", packName);
	}

	return *_packages[packName];
}

}
