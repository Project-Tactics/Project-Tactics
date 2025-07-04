#include "DemoPhysicsState.h"

#include <Engine/Scene/SceneSystem.h>

#include <Libs/Input/InputSystem.h>

namespace tactics {

DemoPhysicsState::DemoPhysicsState(ServiceLocator& serviceLocator) : SampleState(serviceLocator) {}

FsmAction DemoPhysicsState::enter() {
	auto& sceneSystem = getService<SceneSystem>();
	sceneSystem.createEntity("Camera"_id, "defaultCamera"_id);
	sceneSystem.createEntity("Plane"_id, "defaultPlane"_id);
	auto cubeEntity = sceneSystem.createEntity("Cube"_id, "defaultCube"_id);
	return FsmAction::none();
}

void DemoPhysicsState::exit() {}

FsmAction DemoPhysicsState::update() {
	auto& inputSystem = getService<InputSystem>();
	if (inputSystem.checkAction("exitFromState")) {
		return FsmAction::transition("exit"_id);
	}

	return FsmAction::none();
}

} // namespace tactics
