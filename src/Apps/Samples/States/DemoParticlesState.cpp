#include "DemoParticlesState.h"

#include "../Component/RotateAroundPoint.h"

#include <Libs/Ecs/Component/ParticleEmitterComponent.h>
#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Input/InputSystem.h>
#include <Libs/Rendering/Particle/ParticleSystem.h>

namespace tactics {

FsmAction DemoParticlesState::enter() {
	using namespace component;

	auto& sceneSystem = getService<SceneSystem>();
	sceneSystem.createEntity("camera"_id, "particlesCamera"_id);

	//auto& particleSystem = getService<ParticleSystem>();

	sceneSystem.createEntity("fire"_id, "fireEffect"_id);
	sceneSystem.createEntity("ember"_id, "emberEffect"_id);

	/*
	float xSpacing = 0;
	float ySpacing = 0;
	int side = 1;
	float xOffset = (side / 2) * xSpacing - xSpacing / 2;
	float yOffset = (side / 2) * ySpacing - ySpacing / 2;
	for (auto i = 0; i < side * side; ++i) {
		auto entity = sceneSystem.createEntity("defaultGameObject"_id, "defaultGameObject"_id);
		// create x and y coordinates based on index
		auto x = i % side;
		auto y = i / side;
		entity.getComponent<Transform>().setPosition(glm::vec3(x * xSpacing - xOffset, y * ySpacing - yOffset, 0));

		auto effectId = entity.getComponent<component::ParticleEmitter>().maybeEffectId;
		auto config = particleSystem.getEffectConfig(*effectId);
		auto& colorOverLifetime = std::get<firebolt::ColorOverLifetime>(config.modifiers[0]);
		// change start color from blue to red based on x coordinate
		colorOverLifetime.startColor = glm::vec4(0, 0, 1, 1) * (1 - x / static_cast<float>(side - 1)) +
									   glm::vec4(1, 0, 0, 1) * (x / static_cast<float>(side - 1));
		colorOverLifetime.endColor = glm::vec4(0, 0, 1, 1) * (1 - x / static_cast<float>(side - 1)) +
									 glm::vec4(1, 0, 0, 1) * (x / static_cast<float>(side - 1));
		particleSystem.updateEffectConfig(*effectId, config);
	}
	*/
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

	auto& sceneSystem = getService<SceneSystem>();
	component::RotateAroundPointSystem::update(sceneSystem.getRegistry());

	return FsmAction::none();
}

} // namespace tactics
