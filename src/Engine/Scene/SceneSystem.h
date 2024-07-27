#pragma once

#include <Libs/Ecs/Entity.h>
#include <Libs/Resource/Resource.h>
#include <Libs/Utility/Color.h>
#include <Libs/Utility/Math.h>

namespace tactics {
namespace resource {
class ResourceSystem;
}

class EntityComponentSystem;
class Scene;

class SceneSystem {
public:
	SceneSystem(EntityComponentSystem& ecs, resource::ResourceSystem& resourceSystem);
	~SceneSystem();

	void clearScene(bool clearCameras = false);

	[[nodiscard]] const entt::registry& getRegistry() const;
	[[nodiscard]] entt::registry& getRegistry();

	Entity createViewport(const glm::vec2& topLeft, const glm::vec2& size, const glm::vec4& clearColor = Color::black);

	Entity createCamera(const HashId& name,
						const glm::vec3& position,
						const glm::vec3& direction,
						const glm::vec3& up,
						float fov,
						float near,
						float far);

	Entity createEntity(const HashId& name,
						const glm::vec3& position,
						const HashId& meshName,
						const std::vector<HashId>& materials,
						const glm::quat& rotation = Quaternion::identity,
						const glm::vec3& scale = Vector3::one);

	Entity createEntity(const HashId& name, const HashId& prefabName);

	[[nodiscard]] Entity getEntityByName(const HashId& name);
	[[nodiscard]] Entity getCurrentCamera();

private:
	void _onMeshConstructed(entt::registry& registry, entt::entity entity);
	void _onMeshUpdated(entt::registry& registry, entt::entity entity);
	void _onSpriteAnimationConstructed(entt::registry& registry, entt::entity entity);
	void _onSpriteAnimationUpdated(entt::registry& registry, entt::entity entity);
	void _updateAlphaBlendFlags(entt::registry& registry, entt::entity entity);
	void _onCurrentCameraConstructed(entt::registry& registry, entt::entity entity);
	void _onCurrentViewportConstructed(entt::registry& registry, entt::entity entity);

	EntityComponentSystem& _ecs;
	resource::ResourceSystem& _resourceSystem;
};

} // namespace tactics
