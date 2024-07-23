#include "RotateAroundPoint.h"

#include <Libs/Utility/Time/EngineTime.h>

namespace tactics::component {

void RotateAroundPoint::defineReflection() {
	using namespace entt::literals;

	componentReflection<RotateAroundPoint>("rotateAroundPoint")
		.data<&RotateAroundPoint::point>("point"_id)
		.data<&RotateAroundPoint::offset>("offset"_id)
		.data<&RotateAroundPoint::distanceFromPoint>("distanceFromPoint"_id)
		.data<&RotateAroundPoint::speed>("speed"_id)
		.data<&RotateAroundPoint::currentAngle>("currentAngle"_id);
}

void RotateAroundPointSystem::update(entt::registry& registry) {
	auto view = registry.view<Transform, RotateAroundPoint>();
	view.each([](Transform& transform, RotateAroundPoint& rotate) {
		rotate.currentAngle += rotate.speed * EngineTime::fixedDeltaTime<float>();
		if (rotate.currentAngle > 2 * Math::PI) {
			rotate.currentAngle = 0;
		}

		float distance = rotate.distanceFromPoint;
		glm::vec3 position =
			glm::vec3(distance * glm::cos(rotate.currentAngle), 0.0f, distance * glm::sin(rotate.currentAngle));

		transform.setPosition(position + rotate.offset);
		transform.lookAt(rotate.point, Vector3::up);
	});
}

} // namespace tactics::component
