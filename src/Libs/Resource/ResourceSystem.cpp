#include "ResourceSystem.h"

#include "ResourcePack/ResourcePackManager.h"

#include <Libs/Utility/Exception.h>
#include <Libs/Utility/Log/Log.h>

namespace tactics::resource {

ResourceSystem::ResourceSystem(FileSystem& fileSystem)
	: _resourcePackManager(std::make_unique<ResourcePackManager>(fileSystem, *this)) {
}

ResourceSystem::~ResourceSystem() {
}

void ResourceSystem::loadPackDefinition(const std::filesystem::path& definitionPath) {
	_resourcePackManager->loadPackDefinition(definitionPath);
}

void ResourceSystem::loadPack(const hash_string& packName) {
	_resourcePackManager->loadPack(packName);
}

void ResourceSystem::unloadPack(const hash_string& packName) {
	_resourcePackManager->unloadPack(packName);
}

void ResourceSystem::createManualPack(const hash_string& packName) {
	_resourcePackManager->createPack(packName, true);
}

void ResourceSystem::forEachManager(const std::function<void(const BaseResourceManager&)>& callback) {
	for (auto&& [type, manager] : _resourceManagers) {
		callback(*manager);
	}
}

void ResourceSystem::forEachResource(const std::function<void(const Pack&, const PackGroup&, const ResourceInfo&)>& callback) {
	_resourcePackManager->forEachPack([&] (const Pack& pack) {
		pack.forEachResource(callback);
	});
}

void ResourceSystem::forEachPack(const std::function<void(const Pack&)>& callback) {
	_resourcePackManager->forEachPack(callback);
}

void ResourceSystem::registerManager(std::unique_ptr<BaseResourceManager> resourceManager) {
	auto type = resourceManager->getType();
	if (_resourceManagers.contains(type)) {
		LOG_ERROR(Log::Resource, "Can't register a new Resource Type Manager for resource of type {}. A manager is already registered.", type);
	}

	_resourceManagers.insert({type, std::move(resourceManager)});
}

void ResourceSystem::_unregisterManager(std::unique_ptr<BaseResourceManager> resourceManager) {
	auto itr = _resourceManagers.find(resourceManager->getType());
	if (itr == _resourceManagers.end()) {
		LOG_ERROR(Log::Resource, "Can't unregister a Resource Type Manager for resource of type {}. No manager is registered.", resourceManager->getType());
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
	return const_cast<ResourceSystem*>(this)->getManager(resourceType);
}

std::shared_ptr<BaseResource> ResourceSystem::getResource(ResourceType resourceType, const hash_string& name) const {
	return _getManager(resourceType)->getResource(name);
}

std::shared_ptr<BaseResource> ResourceSystem::getResource(ResourceType resourceType, ResourceId id) const {
	return _getManager(resourceType)->getResource(id);
}

void ResourceSystem::loadExternalResource(const hash_string& packName, std::shared_ptr<BaseResource> resource) {
	_resourcePackManager->loadExternalResource(packName, resource);
}

void ResourceSystem::_loadExternalResource(const hash_string& packName, const hash_string& resourceName, ResourceType resourceType, const nlohmann::json& data) {
	_resourcePackManager->loadExternalResource(packName, resourceName, resourceType, data);
}

}
