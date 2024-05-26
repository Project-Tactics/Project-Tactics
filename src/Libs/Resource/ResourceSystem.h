#pragma once

#include "Resource.h"
#include "ResourceManager.h"
#include "ResourcePathHelper.h"

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

class ResourceSystem {
public:
	ResourceSystem(std::string_view relativeDataPath);
	~ResourceSystem();

	void loadResourcePackDefinition(std::string_view definitionPath);
	void loadResourcePack(std::string_view packName);
	void unloadResourcePack(std::string_view packName);

	template<typename TResource>
	TResource& getResource(std::string_view name) {
		return static_cast<TResource&>(_getManager<TResource>()->getResource(name));
	}

	template<typename TResource>
	TResource& getResource(ResourceId id) {
		return static_cast<TResource&>(_getManager<TResource>()->getResource(id));
	}

	template<typename TResourceManager>
	void registerManager() {
		auto manager = std::make_unique<TResourceManager>(_resourcePathHelper);
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

	void cleanupResources();

private:
	void _registerManager(std::unique_ptr<BaseResourceManager> resourceManager);
	void _unregisterManager(std::unique_ptr<BaseResourceManager> resourceManager);

	template<typename TResource>
	BaseResourceManager* _getManager() {
		return _resourceManagers[TResource::TYPE].get();
	}

	ResourcePathHelper _resourcePathHelper;
	std::unique_ptr<ResourcePackManager> _resourcePackManager;
	std::unordered_map<ResourceType, std::unique_ptr<BaseResourceManager>> _resourceManagers;
	std::filesystem::path _absoluteDataPath;
};

}
