#pragma once

#include "Rectangle2DCollider.h"
#include "TranslateItem.h"

#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Ecs/EntityUtilities.h>
#include <Libs/Utility/Reflection.h>

#include <glm/glm.hpp>

namespace tactics::component {
struct BallMovement {
	float speed;
	glm::vec3 axis;

	REFLECT(BallMovement, speed, axis)
};

class BallMovementSystem {
public:
	static void update(const ecs_view<Transform, TranslateItem, BallMovement>& view,
					   const ecs_view<Transform, Rectangle2DCollider>& viewCollider);
	static void _updateCollisionWithWall(Transform& transform, TranslateItem& translateItem);
	static void _updateCollisionWithPlayer(Transform& ballTransform,
										   TranslateItem& ballTranslate,
										   Transform& playerTransform,
										   Rectangle2DCollider& playerCollider);
};
} // namespace tactics::component
