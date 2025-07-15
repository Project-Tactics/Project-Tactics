#pragma once

#include <Libs/Utility/Json/JsonSerialization.h>

namespace tactics::physics {

enum class PhysicsMotionType {
	Static,
	Kinematic,
	Dynamic
};

enum class PhysicsLayer {
	Static = 0,
	Dynamic = 1
};

} // namespace tactics::physics

FORMAT_STR(tactics::physics::PhysicsMotionType)
FORMAT_STR(tactics::physics::PhysicsLayer)
