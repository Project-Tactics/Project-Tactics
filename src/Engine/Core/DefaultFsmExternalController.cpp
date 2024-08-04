#include "DefaultFsmExternalController.h"

#include <Libs/Fsm/FsmBuilder.h>

#include <SDL_events.h>

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

bool DefaultFsmExternalController::onEvent(const SDL_Event& event) {
	if (event.type == SDL_QUIT) {
		_nextTransition = FsmAction::transition(FsmBuilder::appExitRequestTransition);
	}

	return false;
}

} // namespace tactics
