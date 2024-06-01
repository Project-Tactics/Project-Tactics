#include "EmptyState.h"

namespace tactics {

FsmAction EmptyState::enter() {
	return FsmAction::none();
}

void EmptyState::exit() {

}

FsmAction EmptyState::update() {
	if (_exitNextFrame) {
		return FsmAction::transition("proceed");
	}
	return FsmAction::none();
}

bool EmptyState::onKeyPress(SDL_KeyboardEvent& /*event*/) {
	_exitNextFrame = true;
	return true;
}

}
