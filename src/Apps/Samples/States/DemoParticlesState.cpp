#include "DemoParticlesState.h"

#include <Libs/Ecs/Component/ParticleEmitterComponent.h>
#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Input/InputSystem.h>

namespace tactics {

FsmAction DemoParticlesState::enter() {
	using namespace component;

	auto& sceneSystem = getService<SceneSystem>();
	sceneSystem.createEntity("camera"_id, "particlesCamera"_id);

	float xSpacing = 10;
	float ySpacing = 6;
	int side = 6;
	float xOffset = (side / 2) * xSpacing - xSpacing / 2;
	float yOffset = (side / 2) * ySpacing - ySpacing / 2;
	for (auto i = 0; i < side * side; ++i) {
		auto entity = sceneSystem.createEntity("defaultGameObject"_id, "defaultGameObject"_id);
		// create x and y coordinates based on index
		auto x = i % side;
		auto y = i / side;
		entity.getComponent<Transform>().setPosition(glm::vec3(x * xSpacing - xOffset, y * ySpacing - yOffset, 0));
	}

	return FsmAction::none();
}

void DemoParticlesState::exit() {
	auto& sceneSystem = getService<SceneSystem>();
	sceneSystem.clearScene();
}

FsmAction DemoParticlesState::update() {
	auto& inputSystem = getService<InputSystem>();
	if (inputSystem.checkAction("exitFromState")) {
		return FsmAction::transition("exit"_id);
	}

	return FsmAction::none();
}

} // namespace tactics
