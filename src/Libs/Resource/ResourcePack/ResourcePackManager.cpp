#include "ResourcePackManager.h"

#include "../ResourcePathHelper.h"
#include "../ResourceManager.h"

#include <Libs/Utility/Exception.h>

#include <fstream>

namespace tactics::resource {

ResourcePackManager::ResourcePackManager(const ResourcePathHelper& pathHelper, const ResourceManagerProvider& managerProvider)
	: _pathHelper(pathHelper)
	, _managerProvider(managerProvider) {
}

void ResourcePackManager::loadPackDefinition(std::string_view packDefinitionPath) {
	auto path = _pathHelper.makeAbsolutePath(packDefinitionPath);
	std::ifstream fstream(path);
	nlohmann::json data = nlohmann::json::parse(fstream);
	if (!data.contains("packs")) {
		throw Exception("Can't find packs in the resource pack definition file [{}]", packDefinitionPath);
	}

	for (auto&& packData : data["packs"]) {
		auto pack = std::make_unique<Pack>();
		if (!packData.contains("name")) {
			throw Exception("Can't find name for a pack defined in the pack definition file [{}]", packDefinitionPath);
		}
		pack->name = packData["name"].get<std::string>();
		if (_packs.contains(pack->name)) {
			throw Exception("Can't add pack [{}]. A pack with the same name already exists.", pack->name);
		}

		for (auto&& [key, value] : packData.items()) {
			if (key == "name") {
				continue;
			}

			if (!value.is_array()) {
				throw Exception("Can't add pack [{}]. The value for resource type [{}] is not an array.", pack->name, key);
			}

			auto loadInfo = std::make_unique<PackGroup>();
			loadInfo->type = ResourceTypeSerialization::toEnum(key);
			for (auto&& definition : value) {
				loadInfo->descriptors.push_back(definition);
			}
			pack->groups.insert({loadInfo->type, std::move(loadInfo)});
		}
		_packs.insert({pack->name, std::move(pack)});
	}
}

void ResourcePackManager::loadPack(std::string_view packName) {
	auto& resourcePack = _getResourcePack(packName);
	_loadPack(resourcePack);
}

void ResourcePackManager::_loadPack(Pack& pack) {
	for (auto&& [resourceType, group] : pack.groups) {
		auto& manager = _managerProvider(group->type);
		for (auto&& descriptor : group->descriptors) {
			auto resourceId = manager.load(descriptor);
			group->loadedResources.push_back(resourceId);
		}
	}
}

void ResourcePackManager::unloadPack(std::string_view packName) {
	auto& pack = _getResourcePack(packName);
	_unloadPack(pack);
}

void ResourcePackManager::_unloadPack(Pack& pack) {
	for (auto&& [resourceType, group] : pack.groups) {
		for (auto&& resourceId : group->loadedResources) {
			auto& manager = _managerProvider(resourceType);
			manager.unload(resourceId);
		}
		group->loadedResources.clear();
	}
}

void ResourcePackManager::unloadAllPacks() {
	for (auto&& [packName, pack] : _packs) {
		_unloadPack(*pack);
	}
}

ResourcePackManager::Pack& ResourcePackManager::_getResourcePack(std::string_view packName) {
	if (!_packs.contains(packName)) {
		throw Exception("Can't find resource pack with name [{}]. The Resource Pack does not exists", packName);
	}

	return *_packs[packName];
}

}
