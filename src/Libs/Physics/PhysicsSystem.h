#pragma once

#include <array>
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
	 * The ECS must
	 * outlive this system.
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
	std::array<entt::scoped_connection, 5> _connections;
};

} // namespace tactics
