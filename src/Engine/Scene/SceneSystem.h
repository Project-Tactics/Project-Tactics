#pragma once

#include <Libs/Resource/Resource.h>
#include <Libs/Ecs/Entity.h>
#include <Libs/Utility/Math.h>
#include <Libs/Utility/Color.h>

#include <nlohmann/json.hpp>

namespace tactics {
namespace resource {
class ResourceSystem;
}

class EntityComponentSystem;

class SceneSystem {
public:
	SceneSystem(
		EntityComponentSystem& ecs,
		resource::ResourceSystem& resourceSystem);
	~SceneSystem();

	Entity createViewport(
		const glm::vec2& topLeft,
		const glm::vec2& size,
		const glm::vec4& clearColor = Color::black);

	Entity createCamera(
		std::string_view name,
		const glm::vec3& position,
		const glm::vec3& direction,
		const glm::vec3& up,
		float fov,
		float near,
		float far
	);

	Entity createEntity(
		const glm::vec3& position,
		std::string_view meshName,
		std::vector<std::string> materials,
		const glm::quat& rotation = Quaternion::identity,
		const glm::vec3& scale = Vector3::one);

	Entity createEntity(
		std::string_view name,
		std::string_view prefabName
	);

	Entity& getCurrentCamera();

private:
	void _onMeshConstructed(entt::registry& registry, entt::entity entity);
	void _onMeshUpdated(entt::registry& registry, entt::entity entity);
	void _updateAlphaBlendFlags(entt::registry& registry, entt::entity entity);
	void _onCurrentCameraConstructed(entt::registry& registry, entt::entity entity);
	void _onCurrentViewportConstructed(entt::registry& registry, entt::entity entity);

	// TODO(Gerark) Methods used to build an entity from a prefab. Should be moved to a dedicated helper class probably.
	void _buildComponentRecursively(entt::meta_any& instance, const nlohmann::ordered_json& jsonData);
	std::tuple<bool, resource::ResourceType, std::string> _extractResourceInfo(const std::string& value);

	EntityComponentSystem& _ecs;
	resource::ResourceSystem& _resourceSystem;
	Entity _currentCameraEntity;
};

}
