#pragma once

#include "Entity.h"

#include <nlohmann/json.hpp>

namespace tactics {
namespace resource {
class ResourceProvider;
}

class PrefabManager {
public:
	Entity createPrefab(const std::string& name, const nlohmann::ordered_json& json, const resource::ResourceProvider& resourceProvider);
	Entity clonePrefabToRegistry(const std::string& name, const Entity& prefabEntity, entt::registry& destRegistry);

	entt::registry& getRegistry();
	const entt::registry& getRegistry() const;

private:
	void _buildComponentRecursively(entt::meta_any& instance, const nlohmann::ordered_json& jsonData, const resource::ResourceProvider& resourceProvider);

	entt::registry _registry;
};

}
