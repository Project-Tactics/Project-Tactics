#include "BattleCamera.h"

#include <iostream>

namespace tactics::component {

void BattleCamera::defineReflection() {
	componentReflection<BattleCamera>("battleCamera")
		.data<&BattleCamera::rotationSteps>(hash("rotationSteps"))
		.data<&BattleCamera::currentRotation>(hash("currentRotation"))
		.data<&BattleCamera::rotationSpeed>(hash("rotationSpeed"))
		.data<&BattleCamera::nextStep>(hash("nextStep"))
		.data<&BattleCamera::currentStep>(hash("currentStep"))
		.data<&BattleCamera::startingPosition>(hash("startingPosition"));
}

void BattleCameraSystem::update(ecs_view<BattleCamera, Transform> view) {
	for (auto [entity, camera, transform] : view.each()) {
		//std::cout << camera.rotationSteps[0];
	}
}

}
