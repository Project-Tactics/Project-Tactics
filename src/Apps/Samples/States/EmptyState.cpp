#include "EmptyState.h"

#include <Libs/Input/InputSystem.h>

namespace tactics {

FsmAction EmptyState::enter() {
	auto& inputSystem = getService<InputSystem>();
	inputSystem.assignInputMap("emptyStateMap");
	return FsmAction::none();
}

void EmptyState::exit() {}

FsmAction EmptyState::update() {
	auto& inputSystem = getService<InputSystem>();
	if (inputSystem.checkAction("anyKeyPressed")) {
		return FsmAction::transition("proceed"_id);
	}
	return FsmAction::none();
}

} // namespace tactics
