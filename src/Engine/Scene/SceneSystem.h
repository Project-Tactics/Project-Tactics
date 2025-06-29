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

	void drawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, float lifetime = 0.016f);
	void drawBox(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, float lifetime = 0.016f);
	void drawSphere(const glm::vec3& position, float radius, const glm::vec4& color, float lifetime = 0.016f);

	[[nodiscard]] Entity getEntityByName(const HashId& name);
	[[nodiscard]] Entity getCurrentCamera();

private:
	void _onMeshConstructed(entt::registry& registry, entt::entity entity);
	void _onMeshDestroyed(entt::registry& registry, entt::entity entity);
	void _onRenderableConstructed(entt::registry& registry, entt::entity entity);
	void _onRenderableUpdated(entt::registry& registry, entt::entity entity);
	void _onSpriteAnimationConstructed(entt::registry& registry, entt::entity entity);
	void _onSpriteAnimationUpdated(entt::registry& registry, entt::entity entity);
	void _updateAlphaBlendFlags(entt::registry& registry, entt::entity entity);
	void _onCurrentCameraConstructed(entt::registry& registry, entt::entity entity);

	EntityComponentSystem& _ecs;
	resource::ResourceSystem& _resourceSystem;
};

} // namespace tactics
