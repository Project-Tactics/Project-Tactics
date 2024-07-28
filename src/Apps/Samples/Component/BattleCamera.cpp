#include "BattleCamera.h"

#include <Libs/Utility/Time/EngineTime.h>

namespace tactics::component {

void BattleCameraSystem::update(entt::registry& registry) {
	_updateInputs(registry);
	_updateCameras(registry);
}

void BattleCameraSystem::_updateInputs(entt::registry& registry) {
	auto view = registry.view<BattleCamera, BattleCameraInput>();
	for (auto [entity, camera, input] : view.each()) {
		if (input.moveCamera->isTriggered()) {
			auto& value = input.moveCamera->getInputValue();
			if (value.scalar > 0) {
				camera.rotateToNextStep();
			} else {
				camera.rotateToPrevStep();
			}
		}
	}
}

void BattleCameraSystem::_updateCameras(entt::registry& registry) {
	auto view = registry.view<BattleCamera, Transform>();
	for (auto [entity, camera, transform] : view.each()) {
		if (camera.currentStep != camera.nextStep) {
			auto rotation = 0.f;
			camera.rotationTime += camera.rotationSpeed * EngineTime::fixedDeltaTime<float>();
			if (camera.rotationTime >= 1.0f) {
				camera.rotationTime = 0.0f;
				camera.currentStep = camera.nextStep;
				rotation = glm::radians(camera.rotationSteps[camera.currentStep]);
			} else {
				auto startTargetRotation = camera.rotationSteps[camera.currentStep];
				rotation = glm::radians(startTargetRotation +
										(camera.targetRotation - startTargetRotation) * camera.rotationTime);
			}

			float distance = camera.distanceFromOrigin;
			glm::vec3 position = glm::vec3(distance * glm::cos(rotation), 0.0f, distance * glm::sin(rotation));
			transform.setPosition(position + camera.offset);
			transform.lookAt(Vector3::zero, Vector3::up);
		}
	}
}

float BattleCamera::getCurrentRotationDegree() const {
	if (currentStep == nextStep) {
		return rotationSteps[currentStep];
	}

	auto startTargetRotation = rotationSteps[currentStep];
	return startTargetRotation + (targetRotation - startTargetRotation) * rotationTime;
}

void BattleCamera::rotateToNextStep() {
	if (rotationTime == 0.0f) {
		nextStep = (nextStep + 1) % rotationSteps.size();
		targetRotation = rotationSteps[nextStep];
	}
}

void BattleCamera::rotateToPrevStep() {
	if (rotationTime == 0.0f) {
		nextStep = (nextStep - 1) % rotationSteps.size();
		targetRotation = rotationSteps[nextStep];
	}
}

} // namespace tactics::component
