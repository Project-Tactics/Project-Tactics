#include "StartState.h"

namespace tactics {

FsmAction StartState::enter() {
	return FsmAction::transition("proceed");
}

void StartState::exit() {
}

FsmAction StartState::update() {
	return FsmAction::none();
}

}
