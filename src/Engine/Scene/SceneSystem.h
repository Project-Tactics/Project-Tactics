#pragma once

#include <Libs/Utility/Math.h>
#include <Libs/Utility/Color.h>
#include <Libs/Ecs/Entity.h>

namespace tactics {
namespace resource {
class ResourceSystem;
}

class CameraSubSystem;
class EntityComponentSystem;

class SceneSystem {
public:
	SceneSystem(
		EntityComponentSystem& ecs,
		resource::ResourceSystem& resourceSystem);
	~SceneSystem();

	EntityObject createViewport(
		const glm::vec2& topLeft,
		const glm::vec2& size,
		const glm::vec4& clearColor = Color::black);

	EntityObject createCamera(
		const entt::hashed_string& name,
		const glm::vec3& position,
		const glm::vec3& direction,
		const glm::vec3& up,
		float fov,
		float near,
		float far
	);

	EntityObject createEntity(
		const glm::vec3& position,
		std::string_view meshName,
		std::string_view materialName,
		const glm::quat& rotation = Quaternion::identity,
		const glm::vec3& scale = Vector3::one);

	EntityObject& getCurrentCamera();

private:
	void _onMeshConstructed(entt::registry& registry, entt::entity entity);
	void _onMeshUpdated(entt::registry& registry, entt::entity entity);
	void _updateAlphaBlended(entt::registry& registry, entt::entity entity);
	void _onCurrentCameraConstructed(entt::registry& registry, entt::entity entity);
	void _onCurrentViewportConstructed(entt::registry& registry, entt::entity entity);

	EntityComponentSystem& _ecs;
	resource::ResourceSystem& _resourceSystem;
	std::unique_ptr<CameraSubSystem> _cameraSubSystem;
	EntityObject _currentCameraEntity;
};

}
