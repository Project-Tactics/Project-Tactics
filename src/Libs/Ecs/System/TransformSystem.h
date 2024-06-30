#pragma once

#include "../Component/TransformComponent.h"

#include <Libs/Ecs/EntityUtilities.h>

namespace tactics::component {

class TransformSystem {
public:
	static void updateTransformMatrices(ecs_view<Transform>& view);
};

} // namespace tactics::component
