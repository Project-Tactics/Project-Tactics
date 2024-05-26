#include "ResourceSystem.h"

#include "ResourcePack/ResourcePackManager.h"

#include <Libs/Utility/Exception.h>

#include <fmt/format.h>
#include <algorithm>
#include <ranges>

namespace tactics::resource {

ResourceSystem::ResourceSystem(std::string_view relativeDataPath): _resourcePathHelper(relativeDataPath) {
	auto managerProvider = [this] (ResourceType resourceType) -> BaseResourceManager& {
		auto itr = _resourceManagers.find(resourceType);
		if (itr == _resourceManagers.end()) {
			throw Exception("Can't find manager for resource type: {}",
				ResourceTypeSerialization::toString(resourceType));
		}
		return *itr->second;
	};

	_resourcePackManager = std::make_unique<ResourcePackManager>(_resourcePathHelper, managerProvider);
}

ResourceSystem::~ResourceSystem() {
}

void ResourceSystem::loadResourcePackDefinition(std::string_view definitionPath) {
	_resourcePackManager->loadPackDefinition(definitionPath);
}

void ResourceSystem::loadResourcePack(std::string_view resourcePackName) {
	_resourcePackManager->loadPack(resourcePackName);
}

void ResourceSystem::unloadResourcePack(std::string_view resourcePackName) {
	_resourcePackManager->unloadPack(resourcePackName);
}

void ResourceSystem::cleanupResources() {
	_resourcePackManager->unloadAllPacks();
}

void ResourceSystem::_registerManager(std::unique_ptr<BaseResourceManager> resourceManager) {
	auto type = resourceManager->getType();
	if (_resourceManagers.contains(type)) {
		throw Exception("Can't register a new Resource Type Manager for resource of type {}. A manager is already registered.",
			ResourceTypeSerialization::toString(type));
	}

	_resourceManagers.insert({type, std::move(resourceManager)});
}

void ResourceSystem::_unregisterManager(std::unique_ptr<BaseResourceManager> resourceManager) {
	auto itr = _resourceManagers.find(resourceManager->getType());
	if (itr == _resourceManagers.end()) {
		throw Exception("Can't register a new Resource Type Manager for resource of type {}. A manager is already registered.",
			ResourceTypeSerialization::toString(resourceManager->getType()));
	}

	_resourceManagers.erase(itr);
}

}
