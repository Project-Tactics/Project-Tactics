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

	REFLECT(Camera, projectionType, projection, view)
};

struct CurrentCamera {
	REFLECT_EMPTY(CurrentCamera);
};

} // namespace tactics::component

