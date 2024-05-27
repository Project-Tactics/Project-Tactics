#pragma once

#include "Resource.h"
#include "ResourceLoader.h"

#include <Libs/Utility/Exception.h>

#include <unordered_map>
#include <memory>

namespace tactics::resource {

class BaseResourceProvider;

template<typename TResource>
using ResourceMap = std::unordered_map<ResourceId, std::shared_ptr<TResource>>;

/**
* Base class for all the manager of a single Resource type. The user generally doesn't touch this
*/
class BaseResourceManager {
public:
	virtual ~BaseResourceManager() = default;

	virtual ResourceType getType() const = 0;
	virtual void unload(ResourceId resourceId) = 0;
	virtual void unload(std::vector<ResourceId> resourceIds) = 0;
	virtual ResourceId load(const nlohmann::json& descriptor) = 0;
	virtual std::shared_ptr<BaseResource> getResource(std::string_view name) = 0;
	virtual std::shared_ptr<BaseResource> getResource(ResourceId id) = 0;
	virtual std::shared_ptr<BaseResource> getResource(std::string_view name) const = 0;
	virtual std::shared_ptr<BaseResource> getResource(ResourceId id) const = 0;
	virtual void forEachResource(std::function<void(BaseResource&)> callback) = 0;
};

template<typename T>
concept IsResourceLoader = std::is_base_of_v<ResourceLoader, T>;

/**
* A manager class for a single resource type. This is what is mostly used to define a manager for a resource ( like textures, shaders, etc.. )
* Some examples: ResourceManager<Texture, TextureLoader>, ResourceManager<Shader, ShaderLoader>, and so on
*/
template<typename TResource, IsResourceLoader TResourceLoader>
class ResourceManager: public BaseResourceManager {
public:
	ResourceManager(const ResourcePathHelper& pathHelper, const ResourceProvider& resourceProvider): _loader(pathHelper, resourceProvider) {
	}

	std::shared_ptr<BaseResource> getResource(std::string_view name) override final {
		return _getTResource(name);
	}

	std::shared_ptr<BaseResource> getResource(ResourceId id) override final {
		return _getTResource(id);
	}

	std::shared_ptr<BaseResource> getResource(std::string_view name) const final {
		return _getTResource(name);
	}

	std::shared_ptr<BaseResource> getResource(ResourceId id) const final {
		return _getTResource(id);
	}

	ResourceType getType() const override final {
		return TResource::TYPE;
	}

	ResourceId load(const nlohmann::json& descriptor) override final {
		auto resource = _loader.load(descriptor);
		if (!resource) {
			throw Exception("Failed to load resource from descriptor. Resource Type: {} - Descriptor: {}", ResourceTypeSerialization::toString(TResource::TYPE), descriptor.dump());
		}
		auto id = resource->id;
		_registerResource(std::move(resource));
		return id;
	}

	void unload(ResourceId resourceId) override final {
		auto& resource = _getTResource(resourceId);
		_removeResource(resource);
	}

	void unload(std::vector<ResourceId> resourceIds) override final {
		for (auto& resourceId : resourceIds) {
			unload(resourceId);
		}
	}

	void forEachResource(std::function<void(BaseResource&)> callback) override final {
		for (auto& [id, resource] : _resources) {
			callback(*resource);
		}
	}

private:
	std::shared_ptr<TResource>& _getTResource(ResourceId id) {
		return const_cast<std::shared_ptr<TResource>&>(const_cast<const ResourceManager*>(this)->_getTResource(id));
	}

	std::shared_ptr<TResource>& _getTResource(std::string_view name) {
		return const_cast<std::shared_ptr<TResource>&>(const_cast<const ResourceManager*>(this)->_getTResource(name));
	}

	const std::shared_ptr<TResource>& _getTResource(ResourceId id) const {
		if (!_resources.contains(id)) {
			throw Exception("Resource with id \"{}\" does not exist. Can't find resource.", id);
		}

		return _resources.at(id);
	}

	const std::shared_ptr<TResource>& _getTResource(std::string_view name) const {
		auto itr = std::ranges::find_if(_resources, [name] (const auto& pair) {
			return pair.second->name == name;
		});

		if (itr == _resources.end()) {
			throw Exception("Resource with name [{}] does not exist. Can't find resource in [{}] manager.",
				name, ResourceTypeSerialization::toString(getType()));
		}

		return itr->second;
	}

	void _registerResource(std::shared_ptr<TResource> resource) {
		if (_resources.contains(resource->id)) {
			throw Exception("Attempt to register a resource with the same id. Resource Id: {} - Name: {} - Type: {}",
				resource->id, resource->name, ResourceTypeSerialization::toString(resource->type));
		}
		_resources.insert({resource->id, std::move(resource)});
	}

	void _removeResource(std::shared_ptr<TResource>& resource) {
		auto itr = _resources.find(resource->id);
		if (itr == _resources.end()) {
			throw Exception("Attempt to remove a resource which is not registered in the Resource Manager. Resource Id: {} - Name: {} - Type: {}",
				resource->id, resource->name, ResourceTypeSerialization::toString(resource->type));
		}

		auto useCount = resource.use_count();
		if (useCount > 1) {
			throw Exception("Attempt to remove a resource which is still in use. Resource Id: {} - Name: {} - Type: {} - RefCount: {}",
								resource->id, resource->name, ResourceTypeSerialization::toString(resource->type), useCount);
		}

		_resources.erase(itr);
	}

	ResourceMap<TResource> _resources;
	TResourceLoader _loader;
};

}
