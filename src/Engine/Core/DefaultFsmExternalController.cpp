#include "DefaultFsmExternalController.h"

namespace tactics {

FsmAction DefaultFsmExternalController::update(std::string_view currentStateName) {
	_currentStateName = currentStateName;
	return std::exchange(_nextTransition, FsmAction::none());
}

void DefaultFsmExternalController::setNextTransition(std::string nextTransition) {
	_nextTransition = FsmAction::transition(nextTransition);
}

const std::string& DefaultFsmExternalController::getCurrentStateName() const {
	return _currentStateName;
}

}
