#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace tactics {
namespace resource {
class Mesh;
class Material;
}

class EcsSystem: public entt::registry {
public:
	EcsSystem();

	entt::entity createMeshEntity(
		const glm::vec3& position,
		const glm::quat& rotation,
		const glm::vec3& scale,
		std::shared_ptr<resource::Mesh> mesh,
		std::shared_ptr<resource::Material> material
	);

private:
	void _onMeshConstructed(entt::registry& registry, entt::entity entity);
	void _onMeshUpdated(entt::registry& registry, entt::entity entity);

	void _updateAlphaBlended(entt::registry& registry, entt::entity entity);
};

}
