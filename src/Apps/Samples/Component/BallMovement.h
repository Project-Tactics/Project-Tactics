#pragma once

#include "PongMatch.h"
#include "Rectangle2DCollider.h"
#include "RotateItem.h"
#include "TranslateItem.h"

#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Ecs/EntityUtilities.h>
#include <Libs/Utility/Reflection.h>

#include <glm/glm.hpp>

namespace tactics::component {
struct BallMovement {
	float radius = 0.1f;
	float maxSpeed = 4.5f;
	// Speed multiplier applied every time the ball is hit by a player
	float speedMultiplier = 1.1f;
	// Angle in degrees of the bounce when the ball hits the very edge of a player
	float maxBounceAngle = 50.0f;

	COMPONENT(BallMovement, radius, maxSpeed, speedMultiplier, maxBounceAngle)
};

enum class WallType {
	Left,
	Right,
};

struct CollideWithWall {
	WallType wallType;

	COMPONENT(CollideWithWall, wallType)
};

class BallMovementSystem {
public:
	static void update(entt::registry& registry);

private:
	static void _updateCollisionWithWall(entt::registry& registry,
										 entt::entity entity,
										 const PongMatch& match,
										 const BallMovement& ballMovement,
										 Transform& transform,
										 TranslateItem& translateItem);
	static void _updateCollisionWithPlayer(const BallMovement& ballMovement,
										   Transform& ballTransform,
										   TranslateItem& ballTranslate,
										   RotateItem& rotateItem,
										   const Rectangle2DCollider& playerCollider);
};
} // namespace tactics::component
