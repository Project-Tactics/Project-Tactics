#pragma once

#include "Resource.h"

#include <memory>

namespace tactics::resource {

class BaseResourceManager;

class ResourceProvider {
public:
	virtual ~ResourceProvider() = default;

	[[nodiscard]] virtual std::shared_ptr<BaseResource> getResource(ResourceType resourceType,
																	const HashId& name) const = 0;
	[[nodiscard]] virtual std::shared_ptr<BaseResource> getResource(ResourceType resourceType, ResourceId id) const = 0;
	[[nodiscard]] virtual BaseResourceManager& getManager(ResourceType resourceType) const = 0;
	[[nodiscard]] virtual BaseResourceManager& getManager(ResourceType resourceType) = 0;

	template<typename TResource> [[nodiscard]] std::shared_ptr<TResource> getResource(const HashId& name) const {
		return std::dynamic_pointer_cast<TResource>(getResource(TResource::TYPE, name));
	}

	template<typename TResource> [[nodiscard]] std::shared_ptr<TResource> getResource(ResourceId id) const {
		return std::dynamic_pointer_cast<TResource>(getResource(TResource::TYPE, id));
	}
};

} // namespace tactics::resource
