#include "Rectangle2DCollider.h"

#include <Libs/Utility/Time/EngineTime.h>

namespace tactics::component {

void Rectangle2DCollider::updateCenter(const glm::vec3& newCenter) {
	center = newCenter;
}

const bool Rectangle2DCollider::intersect(const glm::vec3& position) {
	if (position.y < center.y + heightFromCenter && position.y > center.y - heightFromCenter) {
		if (position.x < center.x + widthFromCenter && position.x > center.x - widthFromCenter) {
			return true;
		}
	}
	return false;
}

void Rectangle2DColliderSystem::update(const ecs_view<Transform, Rectangle2DCollider>& view) {
	view.each([](auto& transform, auto& collider) { collider.center = transform.getPosition(); });
}

} // namespace tactics::component
