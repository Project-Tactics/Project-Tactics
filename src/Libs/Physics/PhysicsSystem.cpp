#include "PhysicsSystem.h"

#include "JoltConversion.h"
#include "JoltTraceAssert.h"
#include "PhysicsSystemPimpl.h"

#include <Libs/Ecs/Component/PhysicsComponent.h>
#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Ecs/EntityComponentSystem.h>

#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>

JPH_SUPPRESS_WARNING_PUSH

namespace tactics {

using namespace physics;

PhysicsSystem::PhysicsSystem(int tempAllocatorSizeInBytes, EntityComponentSystem& ecs) {
	_initializeSubsystem(tempAllocatorSizeInBytes);

	using namespace component;
	auto& registry = ecs.sceneRegistry();
	registry.on_construct<BoxShape>().connect<&PhysicsSystem::_onBoxShapeCreated>(this);
	registry.on_destroy<BoxShape>().connect<&PhysicsSystem::_onBoxShapeDestroyed>(this);
	registry.on_construct<PhysicsBody>().connect<&PhysicsSystem::_onBodyCreated>(this);
	registry.on_destroy<PhysicsBody>().connect<&PhysicsSystem::_onBodyDestroyed>(this);
	registry.on_update<Transform>().connect<&PhysicsSystem::_onTransformUpdated>(this);
}

PhysicsSystem::~PhysicsSystem() {
	// Unregisters all types with the factory and cleans up the default material
	JPH::UnregisterTypes();

	// Destroy the factory
	delete JPH::Factory::sInstance;
	JPH::Factory::sInstance = nullptr;
}

void PhysicsSystem::_installTraceAndAssertCallbacks() {
	JPH::Trace = joltTrace;
#ifdef JPH_ENABLE_ASSERTS
	JPH::AssertFailed = joltAssert;
#endif
}

void PhysicsSystem::update(float fixedDeltaTime, entt::registry& registry) {
	const int collisionSteps = 1;
	_pimpl->joltPhysicsSystem->Update(fixedDeltaTime,
									  collisionSteps,
									  _pimpl->tempAllocator.get(),
									  _pimpl->jobSystem.get());

	using namespace component;
	auto view = registry.view<Transform, PhysicsBody>();
	auto& bodyInterface = _pimpl->joltPhysicsSystem->GetBodyInterface();
	for (auto [entity, transform, body] : view.each()) {
		if (body.bodyId != 0) {
			JPH::BodyID joltBodyId = toJoltBodyId(body.bodyId);
			JPH::Vec3 position;
			JPH::Quat rotation;
			bodyInterface.GetPositionAndRotation(joltBodyId, position, rotation);
			transform.setPosition(fromJolt(position));
			transform.setRotation(fromJolt(rotation));
		}
	}
}

void PhysicsSystem::_initializeSubsystem(int tempAllocatorSizeInBytes) {
	// Register allocation hook. We'll just let Jolt use malloc/free right now.
	// This needs to be done before any other Jolt function is called.
	JPH::RegisterDefaultAllocator();

	_installTraceAndAssertCallbacks();

	// The factory creates instances of classes based on their name or hash and is mainly used for deserialization
	// of saved data.
	JPH::Factory::sInstance = new JPH::Factory();

	// Register all physics types with the factory and install their collision handlers with the CollisionDispatch
	// class. If you have your own custom shape types you probably need to register their handlers with the
	// CollisionDispatch before calling this function. If you implement your own default material
	// (PhysicsMaterial::sDefault) make sure to initialize it before this function or else this function will create
	// one for you.
	JPH::RegisterTypes();

	_pimpl = std::make_unique<physics::PhysicsSystemPimpl>();
	_pimpl->tempAllocator = std::make_unique<JPH::TempAllocatorImpl>(tempAllocatorSizeInBytes);
	_pimpl->jobSystem =
		std::make_unique<JPH::JobSystemThreadPool>(JPH::cMaxPhysicsJobs,
												   JPH::cMaxPhysicsBarriers,
												   std::max(1u, std::thread::hardware_concurrency() - 1));
	_pimpl->joltPhysicsSystem = std::make_unique<JPH::PhysicsSystem>();
	_pimpl->joltPhysicsSystem->Init(physics::maxBodies,
									physics::numBodyMutexes,
									physics::maxBodyPairs,
									physics::maxContactConstraints,
									*_pimpl.get(),
									*_pimpl.get(),
									*_pimpl.get());

	// A body activation listener gets notified when bodies activate and go to sleep
	// Note that this is called from a job so whatever you do here needs to be thread safe.
	// Registering one is entirely optional.
	_pimpl->joltPhysicsSystem->SetBodyActivationListener(_pimpl.get());

	// A contact listener gets notified when bodies (are about to) collide, and when they separate again.
	// Note that this is called from a job so whatever you do here needs to be thread safe.
	// Registering one is entirely optional.
	_pimpl->joltPhysicsSystem->SetContactListener(_pimpl.get());
}

void PhysicsSystem::_onBoxShapeCreated(entt::registry& registry, entt::entity entity) {
	if (registry.any_of<component::PhysicsBody>(entity)) {
		// If the entity already has a PhysicsBody component, we need to update the shape of the body
		auto& shapeComp = registry.get<component::BoxShape>(entity);
		auto& bodyComp = registry.get<component::PhysicsBody>(entity);
		auto& transform = registry.get<component::Transform>(entity);

		bodyComp.bodyId = _pimpl->createAndAddBody<JPH::BoxShape>(toJolt(transform.getPosition()),
																  toJolt(transform.getRotation()),
																  toJolt(bodyComp.motionType),
																  toJolt(bodyComp.layer),
																  toJolt(shapeComp.halfExtents),
																  bodyComp.convexRadius);
	}
}

void PhysicsSystem::_onBoxShapeDestroyed(entt::registry& /*registry*/, entt::entity /*entity*/) {}

void PhysicsSystem::_onBodyCreated(entt::registry& registry, entt::entity entity) {
	if (registry.any_of<component::BoxShape>(entity)) {
		auto& shapeComp = registry.get<component::BoxShape>(entity);
		auto& bodyComp = registry.get<component::PhysicsBody>(entity);
		auto& transform = registry.get<component::Transform>(entity);
		bodyComp.bodyId = _pimpl->createAndAddBody<JPH::BoxShape>(toJolt(transform.getPosition()),
																  toJolt(transform.getRotation()),
																  toJolt(bodyComp.motionType),
																  toJolt(bodyComp.layer),
																  toJolt(shapeComp.halfExtents),
																  bodyComp.convexRadius);
	}
}

void PhysicsSystem::_onBodyDestroyed(entt::registry& registry, entt::entity entity) {
	auto& bodyComp = registry.get<component::PhysicsBody>(entity);
	// Remove the body from the physics system
	auto& bodyInterface = _pimpl->joltPhysicsSystem->GetBodyInterface();
	auto bodyId = toJoltBodyId(bodyComp.bodyId);
	bodyInterface.RemoveBody(bodyId);
	bodyInterface.DestroyBody(bodyId);
}

void PhysicsSystem::_onTransformUpdated(entt::registry& registry, entt::entity entity) {
	auto& transform = registry.get<component::Transform>(entity);
	if (registry.any_of<component::PhysicsBody>(entity)) {
		auto& bodyComp = registry.get<component::PhysicsBody>(entity);
		if (bodyComp.bodyId != 0) {
			_pimpl->joltPhysicsSystem->GetBodyInterface().SetPositionAndRotation(toJoltBodyId(bodyComp.bodyId),
																				 toJolt(transform.getPosition()),
																				 toJolt(transform.getRotation()),
																				 JPH::EActivation::Activate);
		}
	}
}

} // namespace tactics

JPH_SUPPRESS_WARNING_POP
