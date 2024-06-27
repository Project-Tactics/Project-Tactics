#include "Fsm.h"

#include "FsmExternalController.h"

#include <Libs/Utility/Exception.h>
#include <Libs/Utility/Log/Log.h>

#include <string>
#include <memory>

namespace tactics {

const std::string_view Fsm::exitState = "_Exit";

Fsm::Fsm(FsmStateEntries states, std::string_view startStateName, FsmExternalController* externalController)
	: _states(std::move(states))
	, _startStateName(startStateName)
	, _externalController(externalController) {
	if (!_states.contains(startStateName)) {
		LOG_ERROR(Log::Fsm, "Cannot create Fsm with starting state [{}]. The state does not exist.", _startStateName);
	}
}

void Fsm::update() {
	if (!_currentState) {
		_goToState(_startStateName);
	}

	if (_hasReachedExitState) {
		return;
	}

	if (_performExternalUpdateTransition()) {
		return;
	}

	FsmAction action = _currentState->state->update();
	_performAction<FsmAction>(action);
}

bool Fsm::_performExternalUpdateTransition() {
	if (_externalController) {
		FsmAction action = _externalController->update(_currentState->name);
		if (action.hasTransition()) {
			_executeTransition(action.transitionName());
			return true;
		}
	}
	return false;
}

void Fsm::_goToState(std::string_view stateName) {
	FsmStateEntry* nextState = _getStateByName(stateName);
	if (!nextState) {
		LOG_ERROR(Log::Fsm, "Cannot jump to state [{}]. The state does not exist.", stateName);
		return;
	}

	if (_currentState) {
		LOG_TRACE(Log::Fsm, "Exit state [{}]", _currentState->name);
		_currentState->state->exit();
	}
	_currentState = nextState;
	nextState = nullptr;
	LOG_TRACE(Log::Fsm, "Enter state [{}]", _currentState->name);
	auto action = _currentState->state->enter();
	_performAction<FsmAction>(action);
}

bool Fsm::hasReachedExitState() const {
	return _hasReachedExitState;
}

void Fsm::_executeTransition(std::string_view transition) {
	LOG_TRACE(Log::Fsm, "Executing [{}] transition from state [{}]", transition, _currentState->name);
	FsmTransitions& transitions = _currentState->transitions;
	auto itr = transitions.find(transition.data());
	if (itr == transitions.end()) {
		LOG_ERROR(Log::Fsm, "Cannot execute transition from state [{}]. Transition [{}] does not exist.",
			_currentState->name, transition);
		return;
	}

	FsmTransitionTarget* transitionTarget = nullptr;
	for (auto& target : itr->second) {
		if (!target.condition || target.condition()) {
			transitionTarget = &target;
			break;
		}
	}
	if (!transitionTarget) {
		LOG_ERROR(Log::Fsm, "Cannot execute transition [{}] from state [{}]. No condition evaluated to true", transition, _currentState->name);
		return;
	}

	if (transitionTarget->stateName == exitState) {
		_hasReachedExitState = true;
		LOG_TRACE(Log::Fsm, "Exit state [{}]", _currentState->name);
		_currentState->state->exit();
		return;
	}

	_goToState(transitionTarget->stateName);
}

FsmStateEntry* Fsm::_getStateByName(std::string_view stateName) {
	if (auto itr = _states.find(stateName); itr != _states.end()) {
		return itr->second.get();
	}
	return nullptr;
}

bool Fsm::onEvent(SDL_Event& event) {
	if (_currentState) {
		auto action = _currentState->state->onEvent(event);
		_performAction<FsmEventAction>(action);
		return action.wantsToCaptureInput();
	}
	return false;
}

}
