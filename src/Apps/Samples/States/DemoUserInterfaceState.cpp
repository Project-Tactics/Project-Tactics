#include "DemoUserInterfaceState.h"

#include <Libs/Input/InputSystem.h>

namespace tactics {

FsmAction DemoUserInterfaceState::enter() {
	return FsmAction::none();
}

void DemoUserInterfaceState::exit() {}

FsmAction DemoUserInterfaceState::update() {
	auto& inputSystem = getService<InputSystem>();
	if (inputSystem.checkAction("exitFromState")) {
		return FsmAction::transition("exit"_id);
	}

	return FsmAction::none();
}

} // namespace tactics
