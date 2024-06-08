#pragma once

#include "Entity.h"

using namespace entt::literals;

namespace tactics {

class EntityComponentSystem {
public:
	EntityComponentSystem();

	entt::registry& sceneRegistry();
	entt::registry& prefabRegistry();
	const entt::registry& sceneRegistry() const;
	const entt::registry& prefabRegistry() const;

private:
	entt::registry _sceneRegistry;
	entt::registry _prefabRegistry;
};

}
