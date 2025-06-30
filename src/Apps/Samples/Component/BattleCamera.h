#pragma once

#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Ecs/EntityUtilities.h>
#include <Libs/Resource/Input/InputAction.h>
#include <Libs/Utility/Reflection.h>

namespace tactics::component {

struct BattleCamera {
	float rotationTime{};
	float rotationSpeed{};
	float distanceFromOrigin{};
	float targetRotation{};
	unsigned int nextStep{};
	unsigned int currentStep{};
	glm::vec3 offset{};
	std::vector<float> rotationSteps;

	float getCurrentRotationDegree() const;
	void rotateToNextStep();
	void rotateToPrevStep();

	COMPONENT(BattleCamera,
			  rotationTime,
			  distanceFromOrigin,
			  rotationSpeed,
			  nextStep,
			  currentStep,
			  rotationSteps,
			  offset);
};

struct BattleCameraInput {
	resource::InputAction::Ptr moveCamera;

	COMPONENT(BattleCameraInput, moveCamera);
};

class BattleCameraSystem {
public:
	static void init(entt::registry& registry);
	static void uninit(entt::registry& registry);
	static void update(entt::registry& registry);

private:
	static void _onBattleCameraCreated(entt::registry& registry, entt::entity e);
	static void _updateInputs(entt::registry& registry);
	static void _updateCameras(entt::registry& registry);
};

} // namespace tactics::component
