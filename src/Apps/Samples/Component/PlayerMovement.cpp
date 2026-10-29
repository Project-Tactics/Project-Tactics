#include "PlayerMovement.h"

#include <Libs/Utility/Time/EngineTime.h>

#include <algorithm>

namespace tactics::component {

void PlayerMovementSystem::update(entt::registry& registry) {
	_updateInputs(registry);
}

void PlayerMovementSystem::_updateInputs(entt::registry& registry) {
	auto matchView = registry.view<PongMatch>();
	if (matchView.empty()) {
		return;
	}
	const auto& match = matchView.get<PongMatch>(matchView.front());

	auto view = registry.view<TranslateItem, Transform, PlayerMovement, PlayerMovementInput, Rectangle2DCollider>();
	for (auto [entity, translateItem, transform, playerMovement, input, collider] : view.each()) {
		if (!playerMovement.active) {
			translateItem.axis = Vector3::zero;
			continue;
		}

		// Keep the whole player inside the field
		auto verticalLimit = std::max(0.0f, match.fieldHalfSize.y - collider.heightFromCenter);
		auto position = transform.getPosition();
		position.y = std::clamp(position.y, -verticalLimit, verticalLimit);
		transform.setPosition(position);

		auto& value = input.movePlayer->getInputValue();
		if (value.scalar > 0 && position.y >= verticalLimit) {
			translateItem.axis = Vector3::zero;
		} else if (value.scalar < 0 && position.y <= -verticalLimit) {
			translateItem.axis = Vector3::zero;
		} else {
			translateItem.axis = Vector3::up * value.scalar;
		}
	}
}

} // namespace tactics::component
