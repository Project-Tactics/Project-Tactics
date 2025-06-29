#pragma once

#include <glm/glm.hpp>

namespace tactics::component {

struct DebugLine {
	glm::vec3 start;
	glm::vec3 end;
	glm::vec4 color;
};

struct DebugBox {
	glm::vec3 position;
	glm::vec3 size;
	glm::vec4 color;
};

struct DebugSphere {
	glm::vec3 position;
	float radius{};
	glm::vec4 color;
};

struct DebugDrawingLifetime {
	float lifetime{};
};

} // namespace tactics::component
