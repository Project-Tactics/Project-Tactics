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

	Entity createPrefab(const std::string& name, const nlohmann::ordered_json& jsonData, const resource::ResourceProvider& resourceProvider);
	Entity createEntityFromPrefab(const std::string& name, const Entity& prefabEntity);
	void clearPrefabsRegistry();

	entt::registry& sceneRegistry();
	entt::registry& prefabRegistry();
	const entt::registry& sceneRegistry() const;
	const entt::registry& prefabRegistry() const;

private:
	entt::registry _sceneRegistry;
	PrefabManager _prefabManager;
};

}
