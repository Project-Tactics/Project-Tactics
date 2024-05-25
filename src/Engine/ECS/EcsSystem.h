#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace tactics {
namespace resource {
class Mesh;
class Shader;
class Texture;
}

class EcsSystem {
public:
	EcsSystem();
	entt::registry& getRegistry();
	const entt::registry& getRegistry() const;

	entt::entity createMeshEntity(
		const glm::vec3& position,
		const glm::quat& rotation,
		const glm::vec3& scale,
		resource::Mesh& mesh,
		resource::Shader& shader,
		resource::Texture& texture
	);

private:
	void onMaterialConstructed(entt::registry& registry, entt::entity entity);

	entt::registry _registry;
};

}
