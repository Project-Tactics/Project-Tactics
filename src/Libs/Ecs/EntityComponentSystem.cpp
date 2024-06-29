#include "EntityComponentSystem.h"

#include "Component/ComponentReflection.h"

namespace tactics {

EntityComponentSystem::EntityComponentSystem() {
	ComponentReflection::initializeBasicTypes();
	ComponentReflection::initializeCommonComponents();
}

Entity EntityComponentSystem::createPrefab(const HashId& name, const nlohmann::ordered_json& json, const resource::ResourceProvider& resourceProvider) {
	return _prefabManager.createPrefab(name, json, resourceProvider);
}

Entity EntityComponentSystem::createEntityFromPrefab(const HashId& name, const Entity& prefabEntity) {
	return _prefabManager.clonePrefabToRegistry(name, prefabEntity, _sceneRegistry);
}

void EntityComponentSystem::clearPrefabsRegistry() {
	_prefabManager.getRegistry().clear();
}

entt::registry& EntityComponentSystem::sceneRegistry() {
	return _sceneRegistry;
}

entt::registry& EntityComponentSystem::prefabRegistry() {
	return _prefabManager.getRegistry();
}

const entt::registry& EntityComponentSystem::sceneRegistry() const {
	return _sceneRegistry;
}

const entt::registry& EntityComponentSystem::prefabRegistry() const {
	return _prefabManager.getRegistry();
}

}
