#pragma once

#include "Entity.h"

#include <Libs/Utility/Json/Json.h>

namespace tactics {
namespace resource {
class ResourceProvider;
}

class PrefabManager {
public:
	Entity
	createPrefab(const HashId& name, const ordered_json& json, const resource::ResourceProvider& resourceProvider);
	Entity clonePrefabToRegistry(const HashId& name, const Entity& prefabEntity, entt::registry& destRegistry);

	entt::registry& getRegistry();
	const entt::registry& getRegistry() const;

private:
	entt::registry _registry;
};

} // namespace tactics
