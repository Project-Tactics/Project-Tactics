#include "DemoParticlesState.h"

#include <Libs/Input/InputSystem.h>

namespace tactics {

FsmAction DemoParticlesState::enter() {
	return FsmAction::none();
}

void DemoParticlesState::exit() {}

FsmAction DemoParticlesState::update() {
	auto& inputSystem = getService<InputSystem>();
	if (inputSystem.checkAction("exitFromState")) {
		return FsmAction::transition("exit"_id);
	}

	return FsmAction::none();
}

} // namespace tactics
