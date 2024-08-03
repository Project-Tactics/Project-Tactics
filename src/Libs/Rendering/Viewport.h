#pragma once

#include <Libs/Utility/Color.h>
#include <Libs/Utility/Math.h>

namespace tactics {

struct Viewport {
	glm::vec2 position = Vector2::zero;
	glm::vec2 size = Vector2::zero;
	glm::vec4 clearColor = Color::black;
};

} // namespace tactics
