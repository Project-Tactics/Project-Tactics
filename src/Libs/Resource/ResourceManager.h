#pragma once

#include "Resource.h"

#include <Libs/Utility/Exception.h>

#include <unordered_map>
#include <memory>
#include <sol/sol.hpp>

namespace tactics::resource {

template<typename TResource>
using ResourceMap = std::unordered_map<ResourceId, std::unique_ptr<TResource>>;

/**
* Base class for all the manager of a single Resource type. The user generally doesn't touch this
*/
class ResourceManager {
public:
	virtual ~ResourceManager() = default;

	virtual std::vector<ResourceId> load(sol::reference& luaDefinitionLoader) = 0;
	virtual void unload(ResourceId resourceId) = 0;
	virtual void unload(std::vector<ResourceId> resourceIds) = 0;
	virtual ResourceType getType() const = 0;
};

class ResourcePathHelper;

/**
* A manager class for a single resource type. This is what is mostly used to define a manager for a resource ( like textures, shaders, etc.. )
* Some examples: TResourceManager<Texture>, TResourceManager<Shader>, and so on
*/
template<typename TResource>
class TResourceManager: public ResourceManager {
public:
	TResourceManager(sol::state_view& luaState, const ResourcePathHelper& pathHelper): _luaState(luaState), _pathHelper(pathHelper) {
	}

	TResource& getResource(ResourceId id) {
		if (!_resources.contains(id)) {
			throw Exception("Resource with id \"{}\" does not exist. Can't find resource.", id);
		}

		return *_resources[id];
	}

	TResource& getResource(std::string_view name) {
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

	sol::state_view _luaState;
	const ResourcePathHelper& _pathHelper;
	ResourceMap<TResource> _resources;
};

}
