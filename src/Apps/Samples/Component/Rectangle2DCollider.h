#pragma once

#include "Rectangle2DCollider.h"
#include "TranslateItem.h"

#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Ecs/EntityUtilities.h>
#include <Libs/Utility/Reflection.h>

#include <glm/glm.hpp>

namespace tactics::component {

struct Rectangle2DCollider {
	float heightFromCenter;
	float widthFromCenter;
	glm::vec3 center = Vector3::zero;

	void updateCenter(const glm::vec3& newCenter);
	bool intersect(const glm::vec3& position);
	// True if a circle of the given radius centered in position overlaps the rectangle
	[[nodiscard]] bool overlaps(const glm::vec3& position, float radius) const;

	COMPONENT(Rectangle2DCollider, heightFromCenter, widthFromCenter, center)
};

class Rectangle2DColliderSystem {
public:
	// const float getUpperLimitPosition() const { return getUpperLeftCornerPosition(); }

	static void update(const ecs_view<Transform, Rectangle2DCollider>& view);
};
} // namespace tactics::component
