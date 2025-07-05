#pragma once

#include <entt/entt.hpp>
#include <memory>

namespace tactics {

class EntityComponentSystem;

namespace physics {
class PhysicsSystemPimpl;
}

class PhysicsSystem {
public:
	/*
	 * tempAllocatorSizeInBytes - Size used for temporary allocation during physics update.
	 */
	PhysicsSystem(int tempAllocatorSizeInBytes, EntityComponentSystem& ecs);
	~PhysicsSystem();

	void update(float fixedDeltaTime, entt::registry& registry);

private:
	void _initializeSubsystem(int tempAllocatorSizeInBytes);
	void _installTraceAndAssertCallbacks();

	void _onBoxShapeCreated(entt::registry& registry, entt::entity entity);
	void _onBoxShapeDestroyed(entt::registry& registry, entt::entity entity);
	void _onBodyCreated(entt::registry& registry, entt::entity entity);
	void _onBodyDestroyed(entt::registry& registry, entt::entity entity);
	void _onTransformUpdated(entt::registry& registry, entt::entity entity);

	std::unique_ptr<physics::PhysicsSystemPimpl> _pimpl;
};

} // namespace tactics
