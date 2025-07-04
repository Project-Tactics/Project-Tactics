#pragma once

// clang-format off
// The Jolt.h has to be included before any other Jolt headers
#include <Jolt/Jolt.h>
// clang-format on

#include "JoltConstants.h"

#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/RegisterTypes.h>
#include <memory>

namespace tactics::physics {

class PhysicsSystemPimpl : public JPH::BodyActivationListener,
						   public JPH::ContactListener,
						   public JPH::BroadPhaseLayerInterface,
						   public JPH::ObjectVsBroadPhaseLayerFilter,
						   public JPH::ObjectLayerPairFilter {
public:
	JPH::ValidateResult OnContactValidate(const JPH::Body& inBody1,
										  const JPH::Body& inBody2,
										  JPH::RVec3Arg inBaseOffset,
										  const JPH::CollideShapeResult& inCollisionResult) override;

	void OnContactAdded(const JPH::Body& inBody1,
						const JPH::Body& inBody2,
						const JPH::ContactManifold& inManifold,
						JPH::ContactSettings& ioSettings) override;
	void OnContactPersisted(const JPH::Body& inBody1,
							const JPH::Body& inBody2,
							const JPH::ContactManifold& inManifold,
							JPH::ContactSettings& ioSettings) override;
	void OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair) override;

	void OnBodyActivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData) override;
	void OnBodyDeactivated(const JPH::BodyID& inBodyID, JPH::uint64 inBodyUserData) override;

	JPH::uint GetNumBroadPhaseLayers() const override;
	JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override;
	bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override;
	bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override;

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
	const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override;
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

	// We need a temp allocator for those allocations happening during the physics update.
	std::unique_ptr<JPH::TempAllocatorImpl> tempAllocator;

	// A job system that will execute physics jobs on multiple threads. Typically
	// you would implement the JobSystem interface yourself and let Jolt Physics run on top
	// of your own job scheduler. JobSystemThreadPool is an example implementation.
	std::unique_ptr<JPH::JobSystemThreadPool> jobSystem;

	std::unique_ptr<JPH::PhysicsSystem> joltPhysicsSystem;
};

} // namespace tactics::physics
