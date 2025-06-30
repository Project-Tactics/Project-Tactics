#pragma once

#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Ecs/EntityUtilities.h>
#include <Libs/Utility/Reflection.h>

#include <glm/glm.hpp>

namespace tactics::component {
struct BallBouncing {
	float speed;
	glm::vec3 axis;

	COMPONENT(BallBouncing, speed, axis)
};

class BallBouncingSystem {
public:
	static void update(const ecs_view<Transform, BallBouncing>& view);
};
} // namespace tactics::component
