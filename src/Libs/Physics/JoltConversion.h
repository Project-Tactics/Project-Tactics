#pragma once

#include "JoltConstants.h"
#include "PhysicsTypes.h"

#include <Libs/Utility/Exception.h>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>
#include <Jolt/Physics/Body/MotionType.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace tactics::physics {

static glm::vec3 fromJolt(const JPH::Vec3& vec) {
	return glm::vec3(vec.GetX(), vec.GetY(), vec.GetZ());
}

static JPH::Vec3 toJolt(const glm::vec3& vec) {
	return JPH::Vec3(vec.x, vec.y, vec.z);
}

static JPH::Quat toJolt(const glm::quat& quat) {
	return JPH::Quat(quat.x, quat.y, quat.z, quat.w);
}

static glm::quat fromJolt(const JPH::Quat& quat) {
	return glm::quat(quat.GetW(), quat.GetX(), quat.GetY(), quat.GetZ());
}

static JPH::BodyID toJoltBodyId(std::uint32_t bodyId) {
	return JPH::BodyID(bodyId);
}

static JPH::EMotionType toJolt(PhysicsMotionType motionType) {
	return static_cast<JPH::EMotionType>(motionType);
}

static JPH::ObjectLayer toJolt(PhysicsLayer layer) {
	switch (layer) {
	case PhysicsLayer::Static : return LayerStatic;
	case PhysicsLayer::Dynamic: return LayerDynamic;
	default					  : TACTICS_EXCEPTION("Invalid Layer Provided");
	}

	return LayerDynamic;
}

} // namespace tactics::physics
