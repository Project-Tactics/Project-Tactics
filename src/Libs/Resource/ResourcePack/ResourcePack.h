#pragma once

#include "../Resource.h"

namespace tactics::resource {

class BaseResourceManager;
class ResourceProvider;

/*
 * ResourceInfo is a class that contains metadata information on how to build a resource ( jsonObject ), name, id and a
 * state The state is represented by the resourceId. If the resourceId is valid it means this resource has been loaded
 * This is not the class that represents the resource itself
 */
class ResourceInfo {
public:
	ResourceInfo(HashId name, const ordered_json& data);
	ResourceInfo(HashId name);
	ResourceInfo(BaseResourceManager& manager, std::shared_ptr<BaseResource> resource);
	void load(BaseResourceManager& manager);
	void unload(BaseResourceManager& manager);
	[[nodiscard]] const HashId& getName() const;
	[[nodiscard]] const ordered_json& getData() const;
	[[nodiscard]] std::shared_ptr<BaseResource> getResource() const;
	[[nodiscard]] bool isLoaded() const;

private:
	HashId _name;
	ordered_json _data;
	// If the _resource is nullptr it means the resource has not been loaded
	std::shared_ptr<BaseResource> _resource;
};

/*
 * A PackGroup is a collection of resources of the same type. It contains a list of ResourceInfo objects.
 * The ResourceInfo objects contains mostly metadata information on how to build the resource ( jsonObject ), name, id
 * and a state
 */
class PackGroup {
public:
	PackGroup(ResourceType type);
	void addResource(const HashId& name, const ordered_json& data);
	[[nodiscard]] ResourceType getType() const;
	void load(BaseResourceManager& manager);
	void unload(BaseResourceManager& manager);

	// If the Pack of this group has been manually created the resources can be loaded through these methods
	void loadExternalResource(BaseResourceManager& manager, std::shared_ptr<BaseResource> resource);
	void loadExternalResource(BaseResourceManager& manager, const HashId& name, const json& data);

	void forEachResource(const std::function<void(const ResourceInfo&)>& callback) const;

	unsigned int getResourceCount() const;

private:
	ResourceType _type;
	std::vector<std::unique_ptr<ResourceInfo>> _resources;
};

using PackGroupMap = std::unordered_map<ResourceType, std::unique_ptr<PackGroup>>;

/*
 * A pack is the highest level of abstraction for the Package system.
 * It contains a name, a list to all the type groups of resources that can loaded
 */
class Pack {
public:
	Pack(HashId name, bool manuallyCreated);
	[[nodiscard]] PackGroup& getOrCreatePackGroup(ResourceType type);
	[[nodiscard]] const HashId& getName() const;
	[[nodiscard]] bool isLoaded() const;
	[[nodiscard]] bool isManuallyCreated() const;

	void load(const ResourceProvider& resourceProvider);
	void unload(const ResourceProvider& resourceProvider);

	// If the pack has been manually created the resources can be loaded through these methods
	void loadExternalResource(const ResourceProvider& resourceProvider, std::shared_ptr<BaseResource> resource);
	void loadExternalResource(const ResourceProvider& resourceProvider,
							  const HashId& name,
							  ResourceType type,
							  const json& data);

	void forEachResource(const std::function<void(const Pack&, const PackGroup&, const ResourceInfo&)>& callback) const;
	void forEachGroup(const std::function<void(const PackGroup&)>& callback) const;

	unsigned int getResourceCount() const;

private:
	HashId _name;
	bool _isLoaded{};
	bool _isManuallyCreated{};
	PackGroupMap _groups;
};

} // namespace tactics::resource
