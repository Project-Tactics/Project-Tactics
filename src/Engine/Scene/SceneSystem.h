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
class Scene;

class SceneSystem {
public:
	SceneSystem(
		EntityComponentSystem& ecs,
		resource::ResourceSystem& resourceSystem);
	~SceneSystem();

	void clearScene(bool clearCameras = false);

	[[nodiscard]] const entt::registry& getRegistry() const;
	[[nodiscard]] entt::registry& getRegistry();

	Entity createViewport(
		const glm::vec2& topLeft,
		const glm::vec2& size,
		const glm::vec4& clearColor = Color::black);

	Entity createCamera(
		const hash_string& name,
		const glm::vec3& position,
		const glm::vec3& direction,
		const glm::vec3& up,
		float fov,
		float near,
		float far
	);

	Entity createEntity(
		const glm::vec3& position,
		const hash_string& meshName,
		const std::vector<hash_string>& materials,
		const glm::quat& rotation = Quaternion::identity,
		const glm::vec3& scale = Vector3::one);

	Entity createEntity(
		const hash_string& name,
		const hash_string& prefabName
	);

	[[nodiscard]] Entity getEntityByName(const hash_string& name);
	[[nodiscard]] Entity getCurrentCamera();

private:
	void _onMeshConstructed(entt::registry& registry, entt::entity entity);
	void _onMeshUpdated(entt::registry& registry, entt::entity entity);
	void _updateAlphaBlendFlags(entt::registry& registry, entt::entity entity);
	void _onCurrentCameraConstructed(entt::registry& registry, entt::entity entity);
	void _onCurrentViewportConstructed(entt::registry& registry, entt::entity entity);

	EntityComponentSystem& _ecs;
	resource::ResourceSystem& _resourceSystem;


};

}
