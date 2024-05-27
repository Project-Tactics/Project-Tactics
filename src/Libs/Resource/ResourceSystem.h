#pragma once

#include "Resource.h"
#include "ResourceManager.h"
#include "ResourcePathHelper.h"
#include "ResourceProvider.h"

#include <memory>
#include <string_view>
#include <unordered_map>

namespace tactics::resource {

class ResourcePackManager;

/**
 * @brief A very raw Resource Management class that might be changed in the near future.
 *
 * This class handles the loading and management of various resources such as textures, shaders, etc...
 */

class ResourceSystem: public ResourceProvider {
public:
	ResourceSystem(std::string_view relativeDataPath);
	~ResourceSystem();

	void loadResourcePackDefinition(std::string_view definitionPath);
	void loadResourcePack(std::string_view packName);
	void unloadResourcePack(std::string_view packName);

	template<typename TResource>
	std::shared_ptr<TResource> getResource(std::string_view name) {
		return std::dynamic_pointer_cast<TResource>(_getManager<TResource>()->getResource(name));
	}

	template<typename TResource>
	std::shared_ptr<TResource> getResource(ResourceId id) {
		return std::dynamic_pointer_cast<TResource>(_getManager<TResource>()->getResource(id));
	}

	std::shared_ptr<BaseResource> getResource(ResourceType resourceType, std::string_view name) const override;
	std::shared_ptr<BaseResource> getResource(ResourceType resourceType, ResourceId id) const override;

	template<typename TResourceManager>
	void registerManager() {
		auto manager = std::make_unique<TResourceManager>(_resourcePathHelper, *this);
		_registerManager(std::move(manager));
	}

	template<typename TResourceManager>
	void unregisterManager() {
		auto manager = _getManager<typename TResourceManager::ResourceType>();
		_unregisterManager(manager);
	}

	ResourcePathHelper& getResourcePathHelper() {
		return _resourcePathHelper;
	}

	void forEachResource(std::function<void(BaseResource&)> callback);

private:
	void _registerManager(std::unique_ptr<BaseResourceManager> resourceManager);
	void _unregisterManager(std::unique_ptr<BaseResourceManager> resourceManager);

	template<typename TResource>
	BaseResourceManager* _getManager() {
		return _getManager(TResource::TYPE);
	}

	BaseResourceManager* _getManager(ResourceType resourceType) {
		return _resourceManagers[resourceType].get();
	}

	template<typename TResource>
	const BaseResourceManager* _getManager() const {
		return _getManager(TResource::TYPE);
	}

	const BaseResourceManager* _getManager(ResourceType resourceType) const {
		return _resourceManagers.at(resourceType).get();
	}

	ResourcePathHelper _resourcePathHelper;
	std::unique_ptr<ResourcePackManager> _resourcePackManager;
	std::unordered_map<ResourceType, std::unique_ptr<BaseResourceManager>> _resourceManagers;
	std::filesystem::path _absoluteDataPath;
};

}
