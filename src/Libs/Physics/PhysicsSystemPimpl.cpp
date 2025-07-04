#include "PhysicsSystemPimpl.h"

namespace tactics::physics {

JPH::ValidateResult PhysicsSystemPimpl::OnContactValidate(const JPH::Body&,
														  const JPH::Body&,
														  JPH::RVec3Arg,
														  const JPH::CollideShapeResult&) {
	return JPH::ValidateResult::AcceptContact;
}

void PhysicsSystemPimpl::OnContactAdded(const JPH::Body&,
										const JPH::Body&,
										const JPH::ContactManifold&,
										JPH::ContactSettings&) {}

void PhysicsSystemPimpl::OnContactPersisted(const JPH::Body&,
											const JPH::Body&,
											const JPH::ContactManifold&,
											JPH::ContactSettings&) {}

void PhysicsSystemPimpl::OnContactRemoved(const JPH::SubShapeIDPair&) {}

void PhysicsSystemPimpl::OnBodyActivated(const JPH::BodyID&, JPH::uint64) {}

void PhysicsSystemPimpl::OnBodyDeactivated(const JPH::BodyID&, JPH::uint64) {}

JPH::uint PhysicsSystemPimpl::GetNumBroadPhaseLayers() const {
	return BPLayerCount;
}

JPH::BroadPhaseLayer PhysicsSystemPimpl::GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const {
	JPH_ASSERT(inLayer < LayerCount);
	return LayerConversionTable[inLayer];
}

bool PhysicsSystemPimpl::ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const {
	switch (inObject1) {
	case LayerStatic : return inObject2 == LayerDynamic; // Non moving only collides with moving
	case LayerDynamic: return true;						 // Moving collides with everything
	default			 : JPH_ASSERT(false); return false;
	}
}

bool PhysicsSystemPimpl::ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const {
	switch (inLayer1) {
	case LayerStatic : return inLayer2 == BPLayerDynamic;
	case LayerDynamic: return true;
	default			 : JPH_ASSERT(false); return false;
	}
}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)

const char* PhysicsSystemPimpl::GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const {
	switch ((JPH::BroadPhaseLayer::Type)inLayer) {
	case (JPH::BroadPhaseLayer::Type)physics::BPLayerStatic : return "Static";
	case (JPH::BroadPhaseLayer::Type)physics::BPLayerDynamic: return "Dynamic";
	default													: return "Invalid";
	}
}

#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

} // namespace tactics::physics
