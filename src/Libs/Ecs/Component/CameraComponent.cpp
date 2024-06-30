#include "CameraComponent.h"

#include "../Entity.h"

#include <Libs/Utility/Reflection.h>

namespace tactics::component {

void Camera::defineReflection() {
	entt::meta<ProjectionType>()
		.data<ProjectionType::Perspective>("perspective"_id)
		.data<ProjectionType::Orthographic>("orthographic"_id);

	componentReflection<Camera>("camera").data<&Camera::projectionType>("projectionType"_id);
}

Camera Camera::clone() { return *this; }

void CurrentCamera::defineReflection() { componentReflection<CurrentCamera>("currentCamera"); }

} // namespace tactics::component
