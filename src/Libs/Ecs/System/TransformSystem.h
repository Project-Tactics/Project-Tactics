#pragma once

#include <Libs/Ecs/EntityUtilities.h>

#include "../Component/TransformComponent.h"

namespace tactics::component {

class TransformSystem {
public:
	static void updateTransformMatrices(ecs_view<Transform>& view);
};

}
