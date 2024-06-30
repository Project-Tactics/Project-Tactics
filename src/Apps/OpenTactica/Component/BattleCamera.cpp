#include "BattleCamera.h"

#include <Libs/Utility/Time/EngineTime.h>

#include <iostream>

namespace tactics::component {

void BattleCamera::defineReflection() {
	componentReflection<BattleCamera>("battleCamera")
		.data<&BattleCamera::rotationSteps>("rotationSteps"_id)
		.data<&BattleCamera::rotationTime>("rotationTime"_id)
		.data<&BattleCamera::rotationSpeed>("rotationSpeed"_id)
		.data<&BattleCamera::nextStep>("nextStep"_id)
		.data<&BattleCamera::currentStep>("currentStep"_id);
}

void BattleCameraSystem::update(ecs_view<BattleCamera, Transform> view) {
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
	if (currentStep == nextStep) { return rotationSteps[currentStep]; }

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
