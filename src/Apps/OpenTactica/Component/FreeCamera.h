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
	std::shared_ptr<resource::InputAction> moveAction;
	std::shared_ptr<resource::InputAction> lookAction;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(FreeCamera, player, velocity, speed, sensitivity, pitch, yaw);
	static void defineReflection();
	void deserialize(const resource::ResourceProvider* resourceProvider, const nlohmann::ordered_json& jsonData);
};

class FreeCameraSystem {
public:
	static void update(entt::registry& registry);
};

} // namespace tactics::component
