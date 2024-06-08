#include "CameraComponent.h"

#include "../EntityUtilities.h"
#include "../Entity.h"

namespace tactics::component {

void Camera::defineReflection() {
	entt::meta<ProjectionType>()
		.data<ProjectionType::Perspective>(hash("perspective"))
		.data<ProjectionType::Orthographic>(hash("orthographic"));

	entt::meta<Camera>()
		.type(hash("camera"))
		.func<&Entity::explicitAddComponent<Camera>>(hash("emplace"))
		.data<&Camera::projectionType>(hash("projectionType"));
}

void CurrentCamera::defineReflection() {
	entt::meta<CurrentCamera>()
		.type(hash("currentCamera"))
		.func<&Entity::explicitAddComponent<CurrentCamera>>(hash("emplace"));
}

}
