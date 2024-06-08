#include "EntityComponentSystem.h"

#include "Component/ComponentReflection.h"

namespace tactics {

EntityComponentSystem::EntityComponentSystem() {
	ComponentReflection::initializeBasicTypes();
	ComponentReflection::initializeCommonComponents();
}

entt::registry& EntityComponentSystem::sceneRegistry() {
	return _sceneRegistry;
}

entt::registry& EntityComponentSystem::prefabRegistry() {
	return _prefabRegistry;
}

const entt::registry& EntityComponentSystem::sceneRegistry() const {
	return _sceneRegistry;
}

const entt::registry& EntityComponentSystem::prefabRegistry() const {
	return _prefabRegistry;
}

}
