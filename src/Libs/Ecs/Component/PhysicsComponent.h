#pragma once

#include <Libs/Physics/PhysicsTypes.h>
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
	physics::PhysicsMotionType motionType{physics::PhysicsMotionType::Dynamic};
	physics::PhysicsLayer layer{physics::PhysicsLayer::Dynamic};
	float mass{1.0f};
	float convexRadius{0.05f};
	std::uint32_t bodyId{0};

	COMPONENT(PhysicsBody, mass, motionType, layer, mass, convexRadius)
};

} // namespace tactics::component
