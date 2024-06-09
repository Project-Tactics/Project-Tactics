#include "CameraComponent.h"

#include <Libs/Utility/Reflection.h>

#include "../Entity.h"

namespace tactics::component {

void Camera::defineReflection() {
	entt::meta<ProjectionType>()
		.data<ProjectionType::Perspective>(hash("perspective"))
		.data<ProjectionType::Orthographic>(hash("orthographic"));

	componentReflection<Camera>("camera")
		.data<&Camera::projectionType>(hash("projectionType"));
}

Camera Camera::clone() {
	return *this;
}

void CurrentCamera::defineReflection() {
	componentReflection<CurrentCamera>("currentCamera");
}

}
