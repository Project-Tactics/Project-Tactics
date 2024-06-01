#pragma once

#include <Libs/Ecs/EntityUtilities.h>

#include <Libs/Ecs/Component/TransformComponent.h>

namespace tactics::component {

struct RotateAroundPoint {
	float speed = 0.005f;
	float currentAngle = 0.0f;
	float distanceFromPoint = 10.f;
	glm::vec3 offset = Vector3::zero;
	glm::vec3 point = Vector3::zero;
};

struct RotateAroundPointSystem {
	static void update(ecs_view<Transform, RotateAroundPoint> view);
};

}
