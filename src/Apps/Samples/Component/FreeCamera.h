#pragma once

#include <Libs/Ecs/EntityUtilities.h>
#include <Libs/Resource/Input/InputAction.h>
#include <Libs/Utility/Reflection.h>

#include <glm/glm.hpp>

namespace tactics::component {

struct FreeCamera {
	glm::vec3 velocity{};
	float moveSpeed{1.f};
	float lookSpeed{1.f};
	float lookSensitivity{0.1f};
	float pitch{0.f};
	float yaw{0.f};
	unsigned int player{};
	resource::InputAction::Ptr moveAction;
	resource::InputAction::Ptr lookAction;

	COMPONENT(FreeCamera, player, velocity, moveSpeed, lookSpeed, lookSensitivity, pitch, yaw, moveAction, lookAction);
};

class FreeCameraSystem {
public:
	static void update(entt::registry& registry);
};

} // namespace tactics::component
