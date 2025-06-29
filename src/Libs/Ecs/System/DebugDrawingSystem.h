#pragma once

#include "../EntityUtilities.h"

namespace tactics::component {

class DebugDrawingSystem {
public:
	static void update(entt::registry& registry);
};

} // namespace tactics::component
