#include "SampleSelectionState.h"

namespace tactics {

FsmAction SampleSelectionState::enter() {
	return FsmAction::none();
}

void SampleSelectionState::exit() {}

FsmAction SampleSelectionState::update() {
	return FsmAction::none();
}

} // namespace tactics
