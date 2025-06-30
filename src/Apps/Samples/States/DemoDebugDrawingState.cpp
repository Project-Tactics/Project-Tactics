#include "DemoDebugDrawingState.h"

#include "../Component/RotateAroundPoint.h"

#include <Engine/Scene/SceneSystem.h>

#include <Libs/Input/InputSystem.h>

namespace tactics {

FsmAction DemoDebugDrawingState::enter() {
	auto& sceneSystem = getService<SceneSystem>();
	sceneSystem.createEntity("Camera"_id, "defaultCamera"_id);

	return FsmAction::none();
}

void DemoDebugDrawingState::exit() {}

FsmAction DemoDebugDrawingState::update() {
	auto& inputSystem = getService<InputSystem>();
	if (inputSystem.checkAction("exitFromState")) {
		return FsmAction::transition("exit"_id);
	}

	auto& sceneSystem = getService<SceneSystem>();
	component::RotateAroundPointSystem::update(sceneSystem.getRegistry());

	sceneSystem.drawBox({0, 0, 0}, {1, 1, 1}, Color::red);
	sceneSystem.drawSphere({-2, 0, 0}, 0.5f, Color::green);
	sceneSystem.drawSphere({-3, 0, 0}, 0.25f, Color::red);
	sceneSystem.drawSphere({-3.5, 0, 0}, 0.125f, Color::blue);
	sceneSystem.drawLine({2, 1, 0}, {2, -1, 0}, Color::yellow);
	sceneSystem.drawLine({3, 1, 0}, {3, -1, 0}, Color::gray);
	sceneSystem.drawLine({4, 1, 0}, {4, -1, 0}, Color::magenta);

	return FsmAction::none();
}

} // namespace tactics
