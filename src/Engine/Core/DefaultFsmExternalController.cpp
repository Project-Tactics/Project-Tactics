#include "DefaultFsmExternalController.h"

namespace tactics {

FsmAction DefaultFsmExternalController::update(const HashId& currentStateName) {
	_currentStateName = currentStateName;
	return std::exchange(_nextTransition, FsmAction::none());
}

void DefaultFsmExternalController::setNextTransition(const HashId& nextTransition) {
	_nextTransition = FsmAction::transition(nextTransition);
}

const HashId& DefaultFsmExternalController::getCurrentStateName() const {
	return _currentStateName;
}

}
