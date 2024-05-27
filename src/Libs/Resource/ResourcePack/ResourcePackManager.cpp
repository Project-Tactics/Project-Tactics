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
	nlohmann::ordered_json data = nlohmann::ordered_json::parse(fstream);
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

			auto& packGroup = pack->getOrCreatePackGroup(ResourceTypeSerialization::toEnum(key));
			for (auto&& definition : value) {
				packGroup.descriptors.push_back(definition);
			}
		}
		_packs.insert({pack->name, std::move(pack)});
	}
}

void ResourcePackManager::loadPack(std::string_view packName) {
	auto& resourcePack = _getResourcePack(packName);
	if (resourcePack.isManuallyCreated) {
		throw Exception("Can't load pack [{}]. A manually created pack is considered loaded by default and can't be reloaded again.", packName);
	}
	if (resourcePack.isLoaded) {
		throw Exception("Can't load pack [{}]. The pack is already loaded.", packName);
	}
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
	pack.isLoaded = true;
}

void ResourcePackManager::unloadPack(std::string_view packName) {
	auto& pack = _getResourcePack(packName);
	_unloadPack(pack);
}

void ResourcePackManager::createPack(std::string_view packName) {
	if (_packs.contains(packName)) {
		throw Exception("Can't create custom pack [{}]. A pack with the same name already exists.", packName);
	}

	auto pack = std::make_unique<Pack>();
	pack->name = packName;
	pack->isManuallyCreated = true;
	pack->isLoaded = true; // A custom pack is always considered loaded since there's no certainty of what resources it will contain
	_packs.insert({packName, std::move(pack)});
}

void ResourcePackManager::_unloadPack(Pack& pack) {
	for (auto&& [resourceType, group] : pack.groups | std::views::reverse) {
		for (auto&& resourceId : group->loadedResources) {
			auto& manager = _managerProvider(resourceType);
			manager.unload(resourceId);
		}
		group->loadedResources.clear();
	}
	pack.isLoaded = false;
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

void ResourcePackManager::registerResource(std::string_view packName, std::shared_ptr<BaseResource> resource) {
	auto& pack = _getResourcePack(packName);
	if (!pack.isManuallyCreated) {
		throw Exception("Can't register manual resource [{}] of type [{}] to pack [{}]. The pack is not manually created.",
			resource->name, ResourceTypeSerialization::toString(resource->type), packName);
	}
	auto& packGroup = pack.getOrCreatePackGroup(resource->type);
	auto& manager = _managerProvider(resource->type);
	manager.registerResource(resource);
	packGroup.loadedResources.push_back(resource->id);
}

ResourcePackManager::PackGroup& ResourcePackManager::Pack::getOrCreatePackGroup(ResourceType type) {
	if (groups.contains(type)) {
		return *groups[type];
	}

	auto group = std::make_unique<PackGroup>();
	group->type = type;
	groups.insert({type, std::move(group)});
	return *groups[type];
}

void ResourcePackManager::loadResource(std::string_view packName, const nlohmann::json& descriptor, ResourceType type) {
	auto& pack = _getResourcePack(packName);
	if (!pack.isManuallyCreated) {
		throw Exception("Can't load resource of type [{}] to pack [{}]. The pack is not manually created. Descriptor: {}",
			ResourceTypeSerialization::toString(type), packName, descriptor.dump());
	}
	auto& packGroup = pack.getOrCreatePackGroup(type);
	packGroup.descriptors.push_back(descriptor);

	auto& manager = _managerProvider(type);
	auto resourceId = manager.load(descriptor);
	packGroup.loadedResources.push_back(resourceId);
}

}
