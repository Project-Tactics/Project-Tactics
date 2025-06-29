#pragma once

#include <Libs/Utility/Reflection.h>

namespace tactics::component {

enum class RenderType {
	Mesh,
	Particle
};

struct Renderable {
public:
	RenderType type;

	COMPONENT(Renderable, type);
};
} // namespace tactics::component
