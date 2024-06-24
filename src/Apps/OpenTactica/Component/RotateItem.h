#pragma once

#include <Libs/Ecs/EntityUtilities.h>
#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Utility/Reflection.h>

#include <glm/glm.hpp>

namespace tactics::component {

struct RotateItem {
	float speed;
	glm::vec3 axis;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(RotateItem, speed, axis);
	static void defineReflection();
};

class RotateItemSystem {
public:
	static void update(const ecs_view<Transform, RotateItem>& view);
};

}
