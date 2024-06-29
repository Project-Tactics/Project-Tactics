#include "FsmAction.h"

#include <Libs/Utility/Exception.h>

#include <string>

namespace tactics {

FsmAction FsmAction::none() {
	return {};
}

FsmAction FsmAction::transition(const HashId& transitionName) {
	FsmAction result;
	result._transitionName = transitionName;
	return result;
}

bool FsmAction::hasTransition() const {
	return !_transitionName.isEmpty();
}

const HashId& FsmAction::transitionName() const {
	if (_transitionName.isEmpty()) {
		throw TACTICS_EXCEPTION("FsmAction does not have a transition name");
	}
	return _transitionName;
}

FsmEventAction FsmEventAction::none() {
	FsmEventAction result;
	result._captureInput = false;
	return result;
}

FsmEventAction FsmEventAction::capture() {
	FsmEventAction result;
	result._captureInput = true;
	return result;
}

FsmEventAction FsmEventAction::transition(const HashId& transitionName) {
	FsmEventAction result;
	result._captureInput = true;
	result._transitionName = transitionName;
	return result;
}

const HashId& FsmEventAction::transitionName() const {
	if (_transitionName.isEmpty()) {
		throw TACTICS_EXCEPTION("FsmEventAction does not have a transition name");
	}
	return _transitionName;
}

bool FsmEventAction::wantsToCaptureInput() const {
	return _captureInput;
}

bool FsmEventAction::hasTransition() const {
	return !_transitionName.isEmpty();
}

}
