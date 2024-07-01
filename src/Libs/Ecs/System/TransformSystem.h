#pragma once

#include <Libs/Ecs/EntityUtilities.h>

namespace tactics::component {

class TransformSystem {
public:
	static void updateTransformMatrices(entt::registry& registry);
};

} // namespace tactics::component
