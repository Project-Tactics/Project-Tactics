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
	sceneSystem.createEntity("fire"_id, "fireEffect"_id);
	sceneSystem.createEntity("ember"_id, "emberEffect"_id);
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
