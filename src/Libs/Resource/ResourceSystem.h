#pragma once

#include "Resource.h"
#include "ResourceManager.h"
#include "ResourceProvider.h"
#include "ResourcePack/ResourcePack.h"

#include <memory>
#include <string_view>
#include <unordered_map>

namespace tactics {
class FileSystem;
}

namespace tactics::resource {

class ResourcePackManager;

/**
 * This class handles the loading and management of various resources such as textures, shaders, etc...
 * It is responsible for loading resource packs and individual resources.
 * It also provides a way to register and unregister resource managers for different types of resources.
 */

class ResourceSystem: public ResourceProvider {
public:
	ResourceSystem(FileSystem& fileSystem);
	~ResourceSystem();

	void loadPackDefinition(std::string_view definitionPath);
	void loadPack(std::string_view packName);
	void unloadPack(std::string_view packName);
	void createManualPack(std::string_view packName);

	void loadExternalResource(std::string_view packName, std::shared_ptr<BaseResource> resource);

	template<typename TResource>
	void loadExternalResource(std::string_view packName, std::string_view resourceName, const nlohmann::json& data) {
		_loadExternalResource(packName, resourceName, TResource::TYPE, data);
	}

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
	BaseResourceManager& getManager(ResourceType resourceType) const override;
	BaseResourceManager& getManager(ResourceType resourceType) override;

	void registerManager(std::unique_ptr<BaseResourceManager> resourceManager);

	template<typename TResourceManager>
	void unregisterManager() {
		auto manager = _getManager<typename TResourceManager::ResourceType>();
		_unregisterManager(manager);
	}

	void forEachResource(const std::function<void(const Pack&, const PackGroup&, const ResourceInfo&)>& callback);
	void forEachManager(const std::function<void(const BaseResourceManager&)>& callback);
	void forEachPack(const std::function<void(const Pack&)>& callback);

private:
	void _unregisterManager(std::unique_ptr<BaseResourceManager> resourceManager);
	void _loadExternalResource(std::string_view packName, std::string_view resourceName, ResourceType resourceType, const nlohmann::json& data);

	template<typename TResource>
	BaseResourceManager* _getManager() {
		return _getManager(TResource::TYPE);
	}

	BaseResourceManager* _getManager(ResourceType resourceType) {
		if (!_resourceManagers.contains(resourceType)) {
			throw Exception("Can't find manager for resource type: {}", toString(resourceType));
		}
		return _resourceManagers[resourceType].get();
	}

	template<typename TResource>
	const BaseResourceManager* _getManager() const {
		return _getManager(TResource::TYPE);
	}

	const BaseResourceManager* _getManager(ResourceType resourceType) const {
		return _resourceManagers.at(resourceType).get();
	}

	std::unique_ptr<ResourcePackManager> _resourcePackManager;
	std::unordered_map<ResourceType, std::unique_ptr<BaseResourceManager>> _resourceManagers;
	std::filesystem::path _absoluteDataPath;
};

}
