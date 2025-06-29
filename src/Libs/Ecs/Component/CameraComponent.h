#pragma once

#include <Libs/Utility/Reflection.h>

#include <glm/glm.hpp>

namespace tactics::component {

enum ProjectionType {
	Perspective,
	Orthographic
};

struct Camera {
	ProjectionType projectionType = ProjectionType::Perspective;
	glm::mat4 projection;
	glm::mat4 view;

	COMPONENT(Camera, projectionType, projection, view)
};

struct CurrentCamera {
	COMPONENT_TAG(CurrentCamera);
};

} // namespace tactics::component

