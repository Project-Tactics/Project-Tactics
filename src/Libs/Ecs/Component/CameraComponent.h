#pragma once

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
};

struct CurrentCamera {};

}
