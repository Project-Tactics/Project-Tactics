#include "ComponentReflection.h"

#include "TransformComponent.h"
#include "MeshComponent.h"
#include "../EntityUtilities.h"

#include <entt/entt.hpp>

namespace tactics {

void ComponentReflection::InitializeBasicTypes(entt::registry&) {
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
		.data<&glm::quat::x>(hash("x"))
		.data<&glm::quat::y>(hash("y"))
		.data<&glm::quat::z>(hash("z"))
		.data<&glm::quat::w>(hash("w"));
}

void ComponentReflection::InitializeCommonComponents(entt::registry&) {
	using namespace component;
	Transform::defineReflection();
	Mesh::defineReflection();
}

}
