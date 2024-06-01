#pragma once

#include <Libs/Ecs/EntityUtilities.h>
#include <Libs/Ecs/Component/TransformComponent.h>

#include <glm/glm.hpp>

namespace tactics::component {

struct RotateItem {
	float speed;
	glm::vec3 axis;
};

class RotateItemSystem {
public:
	static void update(const ecs_view<Transform, RotateItem>& view);
};

}
