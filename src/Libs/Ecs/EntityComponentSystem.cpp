#include "EntityComponentSystem.h"

#include "Component/ComponentReflection.h"

#include <Libs/Utility/Exception.h>

#include <ranges>

namespace tactics {

EntityComponentSystem::EntityComponentSystem() {
	ComponentReflection::InitializeBasicTypes(*this);
	ComponentReflection::InitializeCommonComponents(*this);
}

entt::registry& EntityComponentSystem::asRegistry() {
	return *this;
}

const entt::registry& EntityComponentSystem::getPrefabRegistry() const {
	return _prefabRegistry;
}

entt::registry& EntityComponentSystem::getPrefabRegistry() {
	return _prefabRegistry;
}

Entity EntityComponentSystem::createPrefab(const std::string& name) {
	return Entity::create(name, &_prefabRegistry);
}

}
