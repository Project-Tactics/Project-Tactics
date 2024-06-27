#include "ResourcePack.h"

#include "../ResourceManager.h"

#include <Libs/Utility/Exception.h>
#include <Libs/Utility/Log/Log.h>

#include <ranges>

namespace tactics::resource {

/// ResourceInfo

ResourceInfo::ResourceInfo(std::string_view name, const nlohmann::ordered_json& data): _name(name), _data(data) {
}

ResourceInfo::ResourceInfo(std::string_view name): _name(name) {
}

ResourceInfo::ResourceInfo(BaseResourceManager& manager, std::shared_ptr<BaseResource> resource): _name(resource->name), _resource(resource) {
	manager.registerResource(resource);
}

void ResourceInfo::load(BaseResourceManager& manager) {
	if (isLoaded()) {
		throw TACTICS_EXCEPTION("Resource [{}] is already loaded. Can't load again.", _name);
	}
	_resource = manager.load(_name, _data);
}

void ResourceInfo::unload(BaseResourceManager& manager) {
	if (!isLoaded()) {
		throw TACTICS_EXCEPTION("Can't unload Resource [{}] because it is not loaded.", _name);
	}
	auto id = _resource->id;
	_resource = nullptr;
	manager.unload(id);
}

[[nodiscard]] const std::string& ResourceInfo::getName() const {
	return _name;
}

[[nodiscard]] const nlohmann::ordered_json& ResourceInfo::getData() const {
	return _data;
}

[[nodiscard]] std::shared_ptr<BaseResource> ResourceInfo::getResource() const {
	return _resource;
}

[[nodiscard]] bool ResourceInfo::isLoaded() const {
	return _resource != nullptr;
}

/// PackGroup

PackGroup::PackGroup(ResourceType type): _type(type) {
}

void PackGroup::addResource(const std::string& name, const nlohmann::ordered_json& data) {
	_resources.push_back(std::make_unique<ResourceInfo>(name, data));
}

[[nodiscard]] ResourceType PackGroup::getType() const {
	return _type;
}

void PackGroup::load(BaseResourceManager& manager) {
	LOG_TRACE(Log::Resource, "Loading resources of type [{}]...", toString(_type));
	for (auto&& resourceInfo : _resources) {
		resourceInfo->load(manager);
	}
}

void PackGroup::unload(BaseResourceManager& manager) {
	for (auto&& resource : _resources) {
		if (resource->isLoaded()) {
			resource->unload(manager);
		}
	}
}

void PackGroup::loadExternalResource(BaseResourceManager& manager, std::shared_ptr<BaseResource> resource) {
	_resources.push_back(std::make_unique<ResourceInfo>(manager, resource));
}

void PackGroup::loadExternalResource(BaseResourceManager& manager, std::string_view name, const nlohmann::json& data) {
	_resources.push_back(std::make_unique<ResourceInfo>(name, data));
	_resources.back()->load(manager);
}

void PackGroup::forEachResource(const std::function<void(const ResourceInfo&)>& callback) const {
	for (auto&& resource : _resources) {
		callback(*resource);
	}
}

unsigned int PackGroup::getResourceCount() const {
	return static_cast<unsigned int>(_resources.size());
}

/// Pack

Pack::Pack(std::string_view name, bool manuallyCreated): _name(name), _isManuallyCreated(manuallyCreated) {
	// A manually created pack is always considered loaded since there's no certainty of what resources it will contain
	// TODO(Gerark) I'm sure there's a better way to deal with this use case. Should be refactored.
	_isLoaded = _isManuallyCreated;
}

const std::string& Pack::getName() const {
	return _name;
}

bool Pack::isLoaded() const {
	return _isLoaded;
}

bool Pack::isManuallyCreated() const {
	return _isManuallyCreated;
}

PackGroup& Pack::getOrCreatePackGroup(ResourceType type) {
	if (_groups.contains(type)) {
		return *_groups[type];
	}

	auto group = std::make_unique<PackGroup>(type);
	_groups.insert({type, std::move(group)});
	return *_groups[type];
}

void Pack::load(const ResourceProvider& resourceProvider) {
	LOG_TRACE(Log::Resource, "Loading Pack [{}] - Started", _name);
	if (_isManuallyCreated) {
		throw TACTICS_EXCEPTION("Can't load pack [{}]. A manually created pack is considered loaded by default and can't be reloaded again.", _name);
	}
	if (_isLoaded) {
		throw TACTICS_EXCEPTION("Can't load pack [{}]. The pack is already loaded.", _name);
	}

	for (auto&& resourceType : resourceTypeLoadingOrder) {
		if (auto itr = _groups.find(resourceType); itr != _groups.end()) {
			auto& manager = resourceProvider.getManager(resourceType);
			itr->second->load(manager);
		}
	}
	_isLoaded = true;
	LOG_TRACE(Log::Resource, "Loading Pack [{}] - Ended", _name);
	LOG_INFO(Log::Resource, "Pack [{}] Loaded", _name);
}

void Pack::unload(const ResourceProvider& resourceProvider) {
	if (!_isLoaded) {
		throw TACTICS_EXCEPTION("Can't unload pack [{}]. The pack has not been loaded yet.", _name);
	}

	for (auto&& resourceType : resourceTypeLoadingOrder | std::views::reverse) {
		if (auto itr = _groups.find(resourceType); itr != _groups.end()) {
			auto& manager = resourceProvider.getManager(resourceType);
			itr->second->unload(manager);
		}
	}
	_isLoaded = false;

	// Clear all resources if the pack is manually created. We can't maintain metadata information since we don't know
	// if resources have been provided externally or through json data. We could probably check if that's the case but
	// for now we'll just clear everything.
	if (_isManuallyCreated) {
		_groups.clear();
	}
}

void Pack::loadExternalResource(const ResourceProvider& resourceProvider, std::shared_ptr<BaseResource> resource) {
	if (!_isManuallyCreated) {
		throw TACTICS_EXCEPTION("Can't register manual resource [{}] of type [{}] to pack [{}]. The pack is not manually created.",
			resource->name, toString(resource->type), _name);
	}

	auto& group = getOrCreatePackGroup(resource->type);
	auto& manager = resourceProvider.getManager(resource->type);
	group.loadExternalResource(manager, resource);
}

void Pack::loadExternalResource(const ResourceProvider& resourceProvider, std::string_view name, ResourceType type, const nlohmann::json& data) {
	if (!isManuallyCreated()) {
		throw TACTICS_EXCEPTION("Can't load resource [{}] of type [{}] to pack [{}]. The pack has not been created manually. Descriptor: {}",
			name, toString(type), _name, data.dump());
	}
	auto& group = getOrCreatePackGroup(type);
	auto& manager = resourceProvider.getManager(type);
	group.loadExternalResource(manager, name, data);
}

void Pack::forEachResource(const std::function<void(const Pack&, const PackGroup&, const ResourceInfo&)>& callback) const {
	for (auto&& [type, group] : _groups) {
		group->forEachResource([this, &group, &callback] (const ResourceInfo& resource) {
			callback(*this, *group, resource);
		});
	}
}

void Pack::forEachGroup(const std::function<void(const PackGroup&)>& callback) const {
	for (auto&& [type, group] : _groups) {
		callback(*group);
	}
}

unsigned int Pack::getResourceCount() const {
	unsigned int count = 0;
	for (auto&& [type, group] : _groups) {
		count += group->getResourceCount();
	}
	return count;
}

}
