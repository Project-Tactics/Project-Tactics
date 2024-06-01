#pragma once

#include <glm/glm.hpp>

namespace tactics::component {

struct Viewport {
	glm::ivec2 topLeft;
	glm::ivec2 size;
	glm::vec4 clearColor;
};

struct CurrentViewport {};

}
