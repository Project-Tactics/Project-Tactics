#pragma once

#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Ecs/EntityUtilities.h>
#include <Libs/Utility/Reflection.h>

namespace tactics::component {

struct RotateAroundPoint {
	float speed = 0.005f;
	float currentAngle = 0.0f;
	float distanceFromPoint = 10.f;
	glm::vec3 offset = Vector3::zero;
	glm::vec3 point = Vector3::zero;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(RotateAroundPoint,
												speed,
												currentAngle,
												distanceFromPoint,
												offset,
												point);
	static void defineReflection();
};

struct RotateAroundPointSystem {
	static void update(entt::registry& view);
};

} // namespace tactics::component
