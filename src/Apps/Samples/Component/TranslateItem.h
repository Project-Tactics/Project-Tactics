#pragma once

#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Ecs/EntityUtilities.h>
#include <Libs/Utility/Reflection.h>

#include <glm/glm.hpp>

namespace tactics::component {
struct TranslateItem {
	float speed;
	glm::vec3 axis;

	REFLECT(TranslateItem, speed, axis);
};

class TranslateItemSystem {
public:
	static void update(const ecs_view<Transform, TranslateItem>& view);
};
} // namespace tactics::component
