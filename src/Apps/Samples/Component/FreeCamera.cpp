#include "FreeCamera.h"

#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Utility/Time/EngineTime.h>

namespace tactics::component {

void FreeCameraSystem::update(entt::registry& registry) {
	auto view = registry.view<FreeCamera, Transform>();
	for (auto [entity, freeCamera, transform] : view.each()) {
		auto sensitivity = freeCamera.sensitivity * EngineTime::fixedDeltaTime<float>();

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
			freeCamera.pitch += -look.x * sensitivity;
			freeCamera.yaw += -look.y * sensitivity;
			transform.setRotation(glm::quat(glm::vec3(freeCamera.yaw, freeCamera.pitch, 0)));
		}

		transform.translate(freeCamera.velocity * freeCamera.speed * EngineTime::fixedDeltaTime<float>());
		freeCamera.velocity *= 0.9f;
	}
}

} // namespace tactics::component
