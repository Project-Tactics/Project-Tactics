#pragma once

#include <Libs/Utility/Reflection.h>

namespace tactics::component {

struct BoxShape {
	glm::vec3 halfExtents{1.0f};

	COMPONENT(BoxShape, halfExtents)
};

struct SphereShape {
	float radius{1.0f};

	COMPONENT(SphereShape, radius)
};

struct CapsuleShape {
	float radius{1.0f};
	float height{1.0f};

	COMPONENT(CapsuleShape, radius, height)
};

struct PhysicsBody {
	float mass{1.0f};

	COMPONENT(PhysicsBody, mass)
};

} // namespace tactics::component
