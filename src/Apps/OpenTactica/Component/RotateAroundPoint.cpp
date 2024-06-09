#include "RotateAroundPoint.h"

#include <Libs/Utility/Reflection.h>

namespace tactics::component {

void RotateAroundPoint::defineReflection() {
	using namespace entt::literals;

	componentReflection<RotateAroundPoint>("rotateAroundPoint")
		.data<&RotateAroundPoint::point>(hash("point"))
		.data<&RotateAroundPoint::offset>(hash("offset"))
		.data<&RotateAroundPoint::distanceFromPoint>(hash("distanceFromPoint"))
		.data<&RotateAroundPoint::speed>(hash("speed"))
		.data<&RotateAroundPoint::currentAngle>(hash("currentAngle"));
}

void RotateAroundPointSystem::update(ecs_view<Transform, RotateAroundPoint> view) {
	view.each([] (Transform& transform, RotateAroundPoint& rotate) {
		rotate.currentAngle += rotate.speed;
		if (rotate.currentAngle > 2 * Math::PI) {
			rotate.currentAngle = 0;
		}

		float distance = rotate.distanceFromPoint;
		glm::vec3 position = glm::vec3(distance * glm::cos(rotate.currentAngle), 0.0f, distance * glm::sin(rotate.currentAngle));

		transform.setPosition(position + rotate.offset);
		transform.lookAt(rotate.point, Vector3::up);
	});
}

}
