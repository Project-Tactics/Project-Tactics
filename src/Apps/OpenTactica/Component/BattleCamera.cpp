#include "BattleCamera.h"

#include <iostream>

namespace tactics::component {

void BattleCamera::defineReflection() {
	componentReflection<BattleCamera>("battleCamera")
		.data<&BattleCamera::rotationSteps>(hash("rotationSteps"))
		.data<&BattleCamera::rotationTime>(hash("rotationTime"))
		.data<&BattleCamera::rotationSpeed>(hash("rotationSpeed"))
		.data<&BattleCamera::nextStep>(hash("nextStep"))
		.data<&BattleCamera::currentStep>(hash("currentStep"));
}

void BattleCameraSystem::update(ecs_view<BattleCamera, Transform> view) {
	for (auto [entity, camera, transform] : view.each()) {
		if (camera.currentStep != camera.nextStep) {

			auto rotation = 0.f;
			camera.rotationTime += camera.rotationSpeed;
			if (camera.rotationTime >= 1.0f) {
				camera.rotationTime = 0.0f;
				camera.currentStep = camera.nextStep;
				rotation = glm::radians(camera.rotationSteps[camera.currentStep]);
			} else {
				auto startTargetRotation = camera.rotationSteps[camera.currentStep];
				auto nextTargetRotation = camera.rotationSteps[camera.nextStep];
				rotation = glm::radians(startTargetRotation + (nextTargetRotation - startTargetRotation) * camera.rotationTime);
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
	auto nextTargetRotation = rotationSteps[nextStep];
	return startTargetRotation + (nextTargetRotation - startTargetRotation) * rotationTime;
}

}
