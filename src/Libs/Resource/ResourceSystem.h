#pragma once

#include "Resource.h"
#include "ResourceManager.h"
#include "ResourceProvider.h"
#include "ResourcePack/ResourcePack.h"

#include <memory>
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

	void loadPackDefinition(const std::filesystem::path& definitionPath);
	void loadPack(const hash_string& packName);
	void unloadPack(const hash_string& packName);
	void createManualPack(const hash_string& packName);

	void loadExternalResource(const hash_string& packName, std::shared_ptr<BaseResource> resource);

	template<typename TResource>
	void loadExternalResource(const hash_string& packName, const hash_string& resourceName, const nlohmann::json& data) {
		_loadExternalResource(packName, resourceName, TResource::TYPE, data);
	}

	template<typename TResource>
	[[nodiscard]] std::shared_ptr<TResource> getResource(const hash_string& name) {
		return std::dynamic_pointer_cast<TResource>(_getManager<TResource>()->getResource(name));
	}

	template<typename TResource>
	[[nodiscard]] std::shared_ptr<TResource> getResource(ResourceId id) {
		return std::dynamic_pointer_cast<TResource>(_getManager<TResource>()->getResource(id));
	}

	[[nodiscard]] std::shared_ptr<BaseResource> getResource(ResourceType resourceType, const hash_string& name) const override;
	[[nodiscard]] std::shared_ptr<BaseResource> getResource(ResourceType resourceType, ResourceId id) const override;
	[[nodiscard]] BaseResourceManager& getManager(ResourceType resourceType) const override;
	[[nodiscard]] BaseResourceManager& getManager(ResourceType resourceType) override;

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
	void _loadExternalResource(const hash_string& packName, const hash_string& resourceName, ResourceType resourceType, const nlohmann::json& data);

	template<typename TResource>
	BaseResourceManager* _getManager() {
		return _getManager(TResource::TYPE);
	}

	BaseResourceManager* _getManager(ResourceType resourceType) {
		return const_cast<BaseResourceManager*>(const_cast<const ResourceSystem*>(this)->_getManager(resourceType));
	}

	template<typename TResource>
	const BaseResourceManager* _getManager() const {
		return _getManager(TResource::TYPE);
	}

	const BaseResourceManager* _getManager(ResourceType resourceType) const {
		if (!_resourceManagers.contains(resourceType)) {
			throw TACTICS_EXCEPTION("Can't find manager for resource type: {}", toString(resourceType));
		}
		return _resourceManagers.at(resourceType).get();
	}

	std::unique_ptr<ResourcePackManager> _resourcePackManager;
	std::unordered_map<ResourceType, std::unique_ptr<BaseResourceManager>> _resourceManagers;
	std::filesystem::path _absoluteDataPath;
};

}
