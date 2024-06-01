#pragma once

#include <glm/glm.hpp>

namespace tactics::component {

struct Camera {
	glm::mat4 projection;
	glm::mat4 view;
};

struct CurrentCamera {};

}
