#pragma once

#include "Entity.h"
#include "PrefabManager.h"

namespace tactics {
namespace resource {
class ResourceProvider;
}

class EntityComponentSystem {
public:
	EntityComponentSystem();

	Entity
	createPrefab(const HashId& name, const ordered_json& jsonData, const resource::ResourceProvider& resourceProvider);
	Entity createEntityFromPrefab(const HashId& name, const Entity& prefabEntity);
	void clearPrefabsRegistry();

	entt::registry& sceneRegistry();
	entt::registry& prefabRegistry();
	const entt::registry& sceneRegistry() const;
	const entt::registry& prefabRegistry() const;

private:
	entt::registry _sceneRegistry;
	PrefabManager _prefabManager;
};

} // namespace tactics
