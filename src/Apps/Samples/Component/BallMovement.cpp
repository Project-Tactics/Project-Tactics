#include "BallMovement.h"

#include "Rectangle2DCollider.h"

#include <Libs/Utility/Time/EngineTime.h>

namespace tactics::component {

void BallMovementSystem::update(const ecs_view<Transform, TranslateItem, BallMovement>& view,
								const ecs_view<Transform, Rectangle2DCollider>& viewCollider) {
	view.each([&viewCollider](auto& ballTransform, auto& translateItem, auto& /* ballMovement*/) {
		_updateCollisionWithWall(ballTransform, translateItem);

		viewCollider.each([&ballTransform, &translateItem](auto& playerTransform, auto& collider) {
			_updateCollisionWithPlayer(ballTransform, translateItem, playerTransform, collider);
		});
	});
}

void BallMovementSystem::_updateCollisionWithWall(Transform& transform, TranslateItem& translateItem) {
	if (transform.getPosition().y > 2.5f) {
		translateItem.axis.y *= -1.0f;
	} else if (transform.getPosition().y < -2.5f) {
		translateItem.axis.y *= -1.0f;
	}

	if (transform.getPosition().x > 5.0f) {
		translateItem.axis.x *= -1.0f;
		transform.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

	} else if (transform.getPosition().x < -5.0f) {
		translateItem.axis.x *= -1.0f;
		transform.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	}
}

void BallMovementSystem::_updateCollisionWithPlayer(Transform& ballTransform,
													TranslateItem& ballTranslate,
													Transform& /*playerTransform*/,
													Rectangle2DCollider& playerCollider) {
	auto ballDir = ballTranslate.axis;
	if (playerCollider.intersect(ballTransform.getPosition())) {
		if (ballDir.x > 0) {
			ballTranslate.axis.x = -1.0f;
		} else {
			ballTranslate.axis.x = 1.0f;
		}
	}

	/*auto ballYPos = ballTransform.getPosition().y;
	auto ballXPos = ballTransform.getPosition().x;
	auto ballDir = ballTranslate.axis;

	if (ballDir.x < 0) {
		auto leftStickCollUpRightXPos = playerTransform.getPosition().x + 0.2f;
		auto leftStickCollUpRightYPos = playerTransform.getPosition().y + 0.5f;
		auto leftStickCollDownRightYPos = playerTransform.getPosition().y - 0.5f;

		if (ballYPos < leftStickCollUpRightYPos && ballYPos > leftStickCollDownRightYPos) {
			if (ballXPos < leftStickCollUpRightXPos + 0.01f && ballXPos > leftStickCollUpRightXPos - 0.01f) {
				ballTranslate.axis.x = 1.0f;
			}
		}
	} else {
		auto rightStickCollUpRightXPos = playerTransform.getPosition().x - 0.2f;
		auto rightStickCollUpRightYPos = playerTransform.getPosition().y + 0.5f;
		auto rightStickCollDownRightYPos = playerTransform.getPosition().y - 0.5f;

		if (ballYPos < rightStickCollUpRightYPos && ballYPos > rightStickCollDownRightYPos) {
			if (ballXPos > rightStickCollUpRightXPos - 0.01f && ballXPos < rightStickCollUpRightXPos + 0.01f) {
				ballTranslate.axis.x = -1.0f;
			}
		}
	}*/
}

} // namespace tactics::component
