#include "ComponentReflection.h"

#include "../EntityUtilities.h"
#include "BillboardComponent.h"
#include "CameraComponent.h"
#include "FrustumComponent.h"
#include "MeshComponent.h"
#include "ParticleEmitterComponent.h"
#include "PhysicsComponent.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"

#include <Libs/Utility/Json/Json.h>
#include <Libs/Utility/Reflection.h>

#include <entt/entt.hpp>

namespace tactics {

void serializeFromAngleAxis(glm::quat& quat, const ordered_json& jsonData) {
	glm::vec3 axis = jsonData.at("axis");
	float angle = jsonData.at("angle");
	quat = glm::angleAxis(glm::radians(angle), axis);
}

void serializeFromLookAt(glm::quat& quat, const ordered_json& jsonData) {
	glm::vec3 from = jsonData.at("from");
	glm::vec3 to = jsonData.at("to");
	quat = glm::quatLookAt(glm::normalize(to - from), Vector3::up);
}

void ComponentReflection::initializeBasicTypes() {
	entt::meta<glm::vec2>().type("vec2"_id).data<&glm::vec2::x>("x"_id).data<&glm::vec2::y>("y"_id);

	entt::meta<glm::vec3>().type("vec3"_id).data<&glm::vec3::x>("x"_id).data<&glm::vec3::y>("y"_id).data<&glm::vec3::z>(
		"z"_id);

	entt::meta<glm::quat>()
		.type("quat"_id)
		.func<&serializeFromAngleAxis>("fromAngleAxis"_id)
		.func<&serializeFromLookAt>("fromLookAt"_id)
		.data<&glm::quat::x>("x"_id)
		.data<&glm::quat::y>("y"_id)
		.data<&glm::quat::z>("z"_id)
		.data<&glm::quat::w>("w"_id);
}

void ComponentReflection::initializeCommonComponents() {
	using namespace component;
	defineReflection<Billboard,
					 Camera,
					 CurrentCamera,
					 Frustum,
					 Mesh,
					 Sprite,
					 SpriteAnimation,
					 Transform,
					 ParticleEmitter,
					 BoxShape,
					 SphereShape,
					 CapsuleShape,
					 PhysicsBody>();
}

} // namespace tactics
