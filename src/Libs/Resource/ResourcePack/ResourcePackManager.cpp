#include "ResourcePackManager.h"

#include "../ResourceManager.h"

#include <Libs/Utility/Exception.h>

#include <fstream>

namespace tactics::resource {

ResourcePackManager::ResourcePackManager(FileSystem& fileSystem, const ResourceProvider& resourceProvider)
	: _fileSystem(fileSystem)
	, _resourceProvider(resourceProvider) {}

void ResourcePackManager::loadPackDefinition(const std::filesystem::path& packDefinitionPath) {
	auto fileHandle = _loadPackDefinition(packDefinitionPath);

	for (auto&& [packName, packData] : fileHandle->getContent().items()) {
		auto& pack = createPack(HashId(packName), false);

		for (auto&& [resourceType, value] : packData.items()) {
			auto& packGroup = pack.getOrCreatePackGroup(fromString<ResourceType>(resourceType));
			for (auto&& [resourceName, data] : value.items()) {
				packGroup.addResource(HashId(resourceName), data);
			}
		}
	}
}

void ResourcePackManager::loadPack(const HashId& packName) {
	auto& pack = _getResourcePack(packName);
	pack.load(_resourceProvider);
}

void ResourcePackManager::unloadPack(const HashId& packName) {
	auto& pack = _getResourcePack(packName);
	pack.unload(_resourceProvider);
}

void ResourcePackManager::removePack(const HashId& packName) {
	auto& pack = _getResourcePack(packName);
	if (pack.isLoaded()) {
		throw TACTICS_EXCEPTION("Can't remove pack [{}]. The pack is still loaded", packName);
	}

	_packs.erase(packName);
}

Pack& ResourcePackManager::createPack(const HashId& packName, bool isManuallyCreated) {
	if (_packs.contains(packName)) {
		throw TACTICS_EXCEPTION("Can't create custom pack [{}]. A pack with the same name already exists.", packName);
	}

	auto pack = std::make_unique<Pack>(packName, isManuallyCreated);
	return *_packs.insert({packName, std::move(pack)}).first->second;
}

Pack& ResourcePackManager::_getResourcePack(const HashId& packName) {
	if (!_packs.contains(packName)) {
		throw TACTICS_EXCEPTION("Can't find resource pack with name [{}]. The Resource Pack does not exists", packName);
	}

	return *_packs[packName];
}

void ResourcePackManager::loadExternalResource(const HashId& packName, std::shared_ptr<BaseResource> resource) {
	auto& pack = _getResourcePack(packName);
	pack.loadExternalResource(_resourceProvider, resource);
}

void ResourcePackManager::loadExternalResource(const HashId& packName,
											   const HashId& resourceName,
											   ResourceType type,
											   const json& data) {
	auto& pack = _getResourcePack(packName);
	pack.loadExternalResource(_resourceProvider, resourceName, type, data);
}

void ResourcePackManager::forEachPack(std::function<void(const Pack&)> callback) {
	for (auto&& [packName, pack] : _packs) {
		callback(*pack);
	}
}

std::unique_ptr<FileHandle<ordered_json>>
ResourcePackManager::_loadPackDefinition(const std::filesystem::path& packDefinitionPath) {
	auto jsonFileHandle = _fileSystem.createJsonFileHandle(packDefinitionPath);
	if (!jsonFileHandle->exists()) {
		throw TACTICS_EXCEPTION("Pack definition file [{}] does not exist", packDefinitionPath.string());
	}

	jsonFileHandle->load();

	if (jsonFileHandle->getContent().empty()) {
		throw TACTICS_EXCEPTION("Pack definition file [{}] is empty", packDefinitionPath.string());
	}
	return jsonFileHandle;
}

} // namespace tactics::resource
