#include "ResourceSystem.h"

#include "ResourcePack/ResourcePackManager.h"

#include <Libs/Utility/Exception.h>
#include <Libs/Utility/Log/Log.h>

namespace tactics::resource {

ResourceSystem::ResourceSystem(FileSystem& fileSystem)
	: _resourcePackManager(std::make_unique<ResourcePackManager>(fileSystem, *this)) {}

ResourceSystem::~ResourceSystem() {}

void ResourceSystem::loadPackDefinition(const std::filesystem::path& definitionPath) {
	_resourcePackManager->loadPackDefinition(definitionPath);
}

void ResourceSystem::loadPack(const HashId& packName) {
	_resourcePackManager->loadPack(packName);
}

void ResourceSystem::unloadPack(const HashId& packName) {
	_resourcePackManager->unloadPack(packName);
}

void ResourceSystem::createManualPack(const HashId& packName) {
	_resourcePackManager->createPack(packName, true);
}

void ResourceSystem::forEachManager(const std::function<void(const BaseResourceManager&)>& callback) {
	for (auto&& [type, manager] : _resourceManagers) {
		callback(*manager);
	}
}

void ResourceSystem::forEachResource(
	const std::function<void(const Pack&, const PackGroup&, const ResourceInfo&)>& callback) {
	_resourcePackManager->forEachPack([&](const Pack& pack) { pack.forEachResource(callback); });
}

void ResourceSystem::forEachPack(const std::function<void(const Pack&)>& callback) {
	_resourcePackManager->forEachPack(callback);
}

void ResourceSystem::registerManager(std::unique_ptr<BaseResourceManager> resourceManager) {
	auto type = resourceManager->getType();
	if (_resourceManagers.contains(type)) {
		LOG_ERROR(
			Log::Resource,
			"Can't register a new Resource Type Manager for resource of type {}. A manager is already registered.",
			type);
	}

	_resourceManagers.insert({type, std::move(resourceManager)});
}

void ResourceSystem::_unregisterManager(std::unique_ptr<BaseResourceManager> resourceManager) {
	auto itr = _resourceManagers.find(resourceManager->getType());
	if (itr == _resourceManagers.end()) {
		LOG_ERROR(Log::Resource,
				  "Can't unregister a Resource Type Manager for resource of type {}. No manager is registered.",
				  resourceManager->getType());
		return;
	}

	_resourceManagers.erase(itr);
}

BaseResourceManager& ResourceSystem::getManager(ResourceType resourceType) const {
	if (auto itr = _resourceManagers.find(resourceType); itr != _resourceManagers.end()) {
		return *itr->second;
	}

	throw TACTICS_EXCEPTION("Can't find manager for resource type: {}", resourceType);
}

BaseResourceManager& ResourceSystem::getManager(ResourceType resourceType) {
	return *_getManager(resourceType);
}

std::shared_ptr<BaseResource> ResourceSystem::getResource(ResourceType resourceType, const HashId& name) const {
	return _getManager(resourceType)->getResource(name);
}

std::shared_ptr<BaseResource> ResourceSystem::getResource(ResourceType resourceType, ResourceId id) const {
	return _getManager(resourceType)->getResource(id);
}

void ResourceSystem::loadExternalResource(const HashId& packName, std::shared_ptr<BaseResource> resource) {
	_resourcePackManager->loadExternalResource(packName, resource);
}

void ResourceSystem::_loadExternalResource(const HashId& packName,
										   const HashId& resourceName,
										   ResourceType resourceType,
										   const json& data) {
	_resourcePackManager->loadExternalResource(packName, resourceName, resourceType, data);
}

} // namespace tactics::resource
