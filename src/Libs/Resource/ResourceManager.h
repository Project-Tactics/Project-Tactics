#pragma once

#include "Resource.h"
#include "ResourceLoader.h"

#include <Libs/Utility/Exception.h>

#include <unordered_map>
#include <memory>

namespace tactics::resource {

template<typename TResource>
using ResourceMap = std::unordered_map<ResourceId, std::unique_ptr<TResource>>;

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
	virtual BaseResource& getResource(std::string_view name) = 0;
	virtual BaseResource& getResource(ResourceId id) = 0;
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
	ResourceManager(const ResourcePathHelper& pathHelper): _loader(pathHelper) {
	}

	BaseResource& getResource(std::string_view name) override {
		return getTResource(name);
	}

	BaseResource& getResource(ResourceId id) override {
		return getTResource(id);
	}

	TResource& getTResource(ResourceId id) {
		if (!_resources.contains(id)) {
			throw Exception("Resource with id \"{}\" does not exist. Can't find resource.", id);
		}

		return *_resources[id];
	}

	TResource& getTResource(std::string_view name) {
		auto itr = std::ranges::find_if(_resources, [name] (const auto& pair) {
			return pair.second->name == name;
		});

		if (itr == _resources.end()) {
			throw Exception("Resource with name \"{}\" does not exist. Can't find resource.", name);
		}

		return *itr->second;
	}

	ResourceType getType() const override {
		return TResource::TYPE;
	}

	ResourceId load(const nlohmann::json& descriptor) override {
		auto resource = _loader.load(descriptor);
		auto id = resource->id;
		_registerResource(std::move(resource));
		return id;
	}

	void unload(ResourceId resourceId) override {
		auto& resource = getTResource(resourceId);
		_removeResource(resource);
	}

	void unload(std::vector<ResourceId> resourceIds) override {
		for (auto& resourceId : resourceIds) {
			unload(resourceId);
		}
	}

protected:

	void _registerResource(std::unique_ptr<TResource> resource) {
		if (_resources.contains(resource->id)) {
			throw Exception("Attempt to register a resource with the same id. Resource Id: {} - Name: {} - Type: {}",
				resource->id, resource->name, ResourceTypeSerialization::toString(resource->type));
		}
		_resources.insert({resource->id, std::move(resource)});
	}

	void _removeResource(TResource& resource) {
		auto itr = _resources.find(resource.id);
		if (itr == _resources.end()) {
			throw Exception("Attempt to remove a resource which is not registered in the Resource Manager. Resource Id: {} - Name: {} - Type: {}",
				resource.id, resource.name, ResourceTypeSerialization::toString(resource.type));
		}

		_resources.erase(itr);
	}

	ResourceMap<TResource> _resources;
	TResourceLoader _loader;
};

}
