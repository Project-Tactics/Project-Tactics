#pragma once

#include "Resource.h"

#include <memory>

namespace tactics::resource {

class BaseResourceManager;

class ResourceProvider {
public:
	virtual ~ResourceProvider() = default;

	virtual std::shared_ptr<BaseResource> getResource(ResourceType resourceType, std::string_view name) const = 0;
	virtual std::shared_ptr<BaseResource> getResource(ResourceType resourceType, ResourceId id) const = 0;
	virtual BaseResourceManager& getManager(ResourceType resourceType) const = 0;
	virtual BaseResourceManager& getManager(ResourceType resourceType) = 0;

	template<typename TResource>
	std::shared_ptr<TResource> getResource(std::string_view name) const {
		return std::dynamic_pointer_cast<TResource>(getResource(TResource::TYPE, name));
	}

	template<typename TResource>
	std::shared_ptr<TResource> getResource(ResourceId id) const {
		return std::dynamic_pointer_cast<TResource>(getResource(TResource::TYPE, id));
	}
};

}
