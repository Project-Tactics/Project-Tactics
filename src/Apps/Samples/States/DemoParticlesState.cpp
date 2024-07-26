#include "DemoParticlesState.h"

namespace tactics {

FsmAction DemoParticlesState::enter() {
	return FsmAction::none();
}

void DemoParticlesState::exit() {}

FsmAction DemoParticlesState::update() {
	return FsmAction::none();
}

} // namespace tactics
