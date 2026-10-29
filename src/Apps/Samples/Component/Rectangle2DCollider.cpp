#include "Rectangle2DCollider.h"

#include <Libs/Utility/Time/EngineTime.h>

#include <algorithm>

namespace tactics::component {

void Rectangle2DCollider::updateCenter(const glm::vec3& newCenter) {
	center = newCenter;
}

bool Rectangle2DCollider::intersect(const glm::vec3& position) {
	if (position.y < center.y + heightFromCenter && position.y > center.y - heightFromCenter) {
		if (position.x < center.x + widthFromCenter && position.x > center.x - widthFromCenter) {
			return true;
		}
	}
	return false;
}

bool Rectangle2DCollider::overlaps(const glm::vec3& position, float radius) const {
	auto closestX = std::clamp(position.x, center.x - widthFromCenter, center.x + widthFromCenter);
	auto closestY = std::clamp(position.y, center.y - heightFromCenter, center.y + heightFromCenter);
	auto distanceX = position.x - closestX;
	auto distanceY = position.y - closestY;
	return distanceX * distanceX + distanceY * distanceY < radius * radius;
}

void Rectangle2DColliderSystem::update(const ecs_view<Transform, Rectangle2DCollider>& view) {
	view.each([](auto& transform, auto& collider) { collider.center = transform.getPosition(); });
}

} // namespace tactics::component
