#pragma once

#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Ecs/EntityUtilities.h>
#include <Libs/Resource/Input/InputAction.h>
#include <Libs/Utility/Math.h>
#include <Libs/Utility/Reflection.h>

#include <array>

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

	REFLECT(BattleCamera,
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

	REFLECT(BattleCameraInput, moveCamera);
};

class BattleCameraSystem {
public:
	static void update(entt::registry& registry);

private:
	static void _updateInputs(entt::registry& registry);
	static void _updateCameras(entt::registry& registry);
};

} // namespace tactics::component
