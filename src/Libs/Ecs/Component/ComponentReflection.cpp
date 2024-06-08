#include "ComponentReflection.h"

#include "TransformComponent.h"
#include "MeshComponent.h"
#include "CameraComponent.h"
#include "FrustumComponent.h"
#include "../EntityUtilities.h"

#include <Libs/Utility/Json/MathJsonSerializer.h>
#include <Libs/Utility/Reflection.h>

#include <entt/entt.hpp>

namespace tactics {

void serializeFromAngleAxis(glm::quat& quat, const nlohmann::ordered_json& jsonData) {
	glm::vec3 axis = jsonData.at("axis");
	float angle = jsonData.at("angle");
	quat = glm::angleAxis(glm::radians(angle), axis);
}

void ComponentReflection::initializeBasicTypes(entt::registry&) {
	entt::meta<glm::vec2>()
		.type(hash("vec2"))
		.data<&glm::vec2::x>(hash("x"))
		.data<&glm::vec2::y>(hash("y"));

	entt::meta<glm::vec3>()
		.type(hash("vec3"))
		.data<&glm::vec3::x>(hash("x"))
		.data<&glm::vec3::y>(hash("y"))
		.data<&glm::vec3::z>(hash("z"));

	entt::meta<glm::quat>()
		.type(hash("quat"))
		.func<&serializeFromAngleAxis>(hash("fromAngleAxis"))
		.data<&glm::quat::x>(hash("x"))
		.data<&glm::quat::y>(hash("y"))
		.data<&glm::quat::z>(hash("z"))
		.data<&glm::quat::w>(hash("w"));
}

void ComponentReflection::initializeCommonComponents(entt::registry&) {
	using namespace component;
	defineComponentsReflection<Transform, Mesh, Camera, Frustum, CurrentCamera>();
}

}
