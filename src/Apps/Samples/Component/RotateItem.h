#pragma once

#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Ecs/EntityUtilities.h>
#include <Libs/Utility/Reflection.h>

#include <glm/glm.hpp>

namespace tactics::component {

struct RotateItem {
	float speed;
	glm::vec3 axis;

	COMPONENT(RotateItem, speed, axis);
};

class RotateItemSystem {
public:
	static void update(entt::registry& registry);
};

} // namespace tactics::component
