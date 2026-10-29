#include "BallMovement.h"

#include <algorithm>
#include <cmath>

namespace tactics::component {

void BallMovementSystem::update(entt::registry& registry) {
	auto matchView = registry.view<PongMatch>();
	if (matchView.empty()) {
		return;
	}
	const auto& match = matchView.get<PongMatch>(matchView.front());

	auto ballView = registry.view<Transform, TranslateItem, BallMovement, RotateItem>();
	auto colliderView = registry.view<Rectangle2DCollider>();
	for (auto [entity, transform, translateItem, ballMovement, rotateItem] : ballView.each()) {
		_updateCollisionWithWall(registry, entity, match, ballMovement, transform, translateItem);

		for (auto [colliderEntity, collider] : colliderView.each()) {
			_updateCollisionWithPlayer(ballMovement, transform, translateItem, rotateItem, collider);
		}
	}
}

void BallMovementSystem::_updateCollisionWithWall(entt::registry& registry,
												  entt::entity entity,
												  const PongMatch& match,
												  const BallMovement& ballMovement,
												  Transform& transform,
												  TranslateItem& translateItem) {
	auto& position = transform.getPosition();
	auto verticalLimit = match.fieldHalfSize.y - ballMovement.radius;

	// Bounce only when moving towards the wall, otherwise the ball could get stuck flipping its direction every tick
	if ((position.y > verticalLimit && translateItem.axis.y > 0) ||
		(position.y < -verticalLimit && translateItem.axis.y < 0)) {
		translateItem.axis.y *= -1.0f;
	}

	if (position.x - ballMovement.radius > match.fieldHalfSize.x) {
		registry.emplace_or_replace<CollideWithWall>(entity, WallType::Right);
	} else if (position.x + ballMovement.radius < -match.fieldHalfSize.x) {
		registry.emplace_or_replace<CollideWithWall>(entity, WallType::Left);
	}
}

void BallMovementSystem::_updateCollisionWithPlayer(const BallMovement& ballMovement,
													Transform& ballTransform,
													TranslateItem& ballTranslate,
													RotateItem& rotateItem,
													const Rectangle2DCollider& playerCollider) {
	auto position = ballTransform.getPosition();
	if (!playerCollider.overlaps(position, ballMovement.radius)) {
		return;
	}

	// Ignore the player if the ball is already moving away from it (e.g. it has just bounced on it)
	auto directionToPlayer = playerCollider.center.x - position.x;
	if (directionToPlayer * ballTranslate.axis.x <= 0) {
		return;
	}

	// The further from the center of the player the ball hits, the steeper the bounce
	auto hitOffset = std::clamp((position.y - playerCollider.center.y) / playerCollider.heightFromCenter, -1.0f, 1.0f);
	auto bounceAngle = glm::radians(hitOffset * ballMovement.maxBounceAngle);
	auto horizontalDirection = directionToPlayer > 0 ? -1.0f : 1.0f;
	ballTranslate.axis = {horizontalDirection * std::cos(bounceAngle), std::sin(bounceAngle), 0.0f};

	if (ballTranslate.speed < ballMovement.maxSpeed) {
		auto newSpeed = std::min(ballTranslate.speed * ballMovement.speedMultiplier, ballMovement.maxSpeed);
		rotateItem.speed *= newSpeed / ballTranslate.speed;
		ballTranslate.speed = newSpeed;
	}

	// Move the ball out of the player collider to avoid hitting it again in the next ticks
	position.x = playerCollider.center.x + horizontalDirection * (playerCollider.widthFromCenter + ballMovement.radius);
	ballTransform.setPosition(position);
}

} // namespace tactics::component
