#include "InputAction.h"

namespace tactics::resource {

bool InputAction::isTriggered(click::PlayerId playerId) const {
	return states[playerId].state == click::InputState::Triggered;
}

const click::ActionValue& InputAction::getInputValue(click::PlayerId playerId) const {
	return states[playerId].value;
}

} // namespace tactics::resource
