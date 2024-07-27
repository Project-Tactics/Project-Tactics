#pragma once

#include <Libs/Ecs/EntityUtilities.h>
#include <Libs/Resource/Input/InputAction.h>
#include <Libs/Utility/Reflection.h>

#include <glm/glm.hpp>

namespace tactics::component {

struct FreeCamera {
	glm::vec3 velocity{};
	float speed{1.f};
	float sensitivity{0.1f};
	float pitch{0.f};
	float yaw{0.f};
	unsigned int player{};
	resource::InputAction::Ptr moveAction;
	resource::InputAction::Ptr lookAction;

	REFLECT(FreeCamera, player, velocity, speed, sensitivity, pitch, yaw, moveAction, lookAction);
};

class FreeCameraSystem {
public:
	static void update(entt::registry& registry);
};

} // namespace tactics::component
