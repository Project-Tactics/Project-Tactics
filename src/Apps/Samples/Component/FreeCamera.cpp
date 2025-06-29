#include "FreeCamera.h"

#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Utility/Time/EngineTime.h>

namespace tactics::component {

void FreeCameraSystem::update(entt::registry& registry) {
	auto view = registry.view<FreeCamera, Transform>();
	for (auto [entity, freeCamera, transform] : view.each()) {
		auto forward = transform.getRotation() * Vector3::forward;
		auto right = transform.getRotation() * Vector3::right;

		auto& state = freeCamera.moveAction->states[freeCamera.player];
		auto& move = state.value.vec3;

		if (move.x != 0.f || move.y != 0.f || move.z != 0.f) {
			freeCamera.velocity += move.x * right + move.y * Vector3::up + move.z * forward;
		}

		state = freeCamera.lookAction->states[freeCamera.player];
		auto& look = state.value.vec3;

		if (look.x != 0.f || look.y != 0.f || look.z != 0.f) {
			freeCamera.pitch += -look.y;
			freeCamera.yaw += -look.x;
		}

		float dt = EngineTime::fixedDeltaTime<float>();
		glm::quat qPitch = glm::angleAxis(freeCamera.pitch * freeCamera.lookSensitivity * dt, Vector3::right);
		glm::quat qYaw = glm::angleAxis(freeCamera.yaw * freeCamera.lookSensitivity * dt, Vector3::up);
		transform.setRotation(glm::lerp(transform.getRotation(), qYaw * qPitch, dt * freeCamera.lookSpeed));
		transform.translate(freeCamera.velocity * freeCamera.moveSpeed * dt);
		freeCamera.velocity *= 0.9f;
	}
}

} // namespace tactics::component
