#include "EmptyState.h"

namespace tactics {

FsmAction EmptyState::enter() {
	return FsmAction::none();
}

void EmptyState::exit() {

}

FsmAction EmptyState::update() {
	return FsmAction::none();
}

FsmEventAction EmptyState::onKeyPress(SDL_KeyboardEvent& /*event*/) {
	return FsmEventAction::transition("proceed"_id);
}

}
