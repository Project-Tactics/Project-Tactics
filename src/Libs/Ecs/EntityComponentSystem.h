#pragma once

#include "Entity.h"

using namespace entt::literals;

namespace tactics {

class EntityComponentSystem: public entt::registry {
public:
	EntityComponentSystem();

	const entt::registry& getPrefabRegistry() const;
	entt::registry& getPrefabRegistry();
	Entity createPrefab(const std::string& name);

	entt::registry& asRegistry();

private:
	// This is a separate registry which is used to store prefabs entities only
	entt::registry _prefabRegistry;
};

}
