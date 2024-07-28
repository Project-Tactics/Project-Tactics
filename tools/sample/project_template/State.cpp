#include "Demo##SAMPLE_NAME##State.h"

#include "../Component/RotateAroundPoint.h"

#include <Engine/Scene/SceneSystem.h>

#include <Libs/Input/InputSystem.h>

namespace tactics {

FsmAction Demo##SAMPLE_NAME##State::enter() {
	auto& sceneSystem = getService<SceneSystem>();
	sceneSystem.createEntity("Camera"_id, "defaultCamera"_id);
	sceneSystem.createEntity("Cube"_id, "defaultCube"_id);
	return FsmAction::none();
}

void Demo##SAMPLE_NAME##State::exit() {}

FsmAction Demo##SAMPLE_NAME##State::update() {
	auto& inputSystem = getService<InputSystem>();
	if (inputSystem.checkAction("exitFromState")) {
		return FsmAction::transition("exit"_id);
	}

	auto& sceneSystem = getService<SceneSystem>();
	component::RotateAroundPointSystem::update(sceneSystem.getRegistry());

	return FsmAction::none();
}

} // namespace tactics
