#include "FsmBuilder.h"
#include "Fsm.h"

#include <Libs/Utility/Exception.h>

namespace tactics {

FsmBuilder& FsmBuilder::state(std::string_view stateName, std::unique_ptr<FsmState> state) {
	if (_states.contains(stateName)) {
		throw TACTICS_EXCEPTION("Can't add state with name [{}]. State already exists", stateName);
	}
	_latestOnTransition.clear();
	auto entry = std::make_unique<FsmStateEntry>();
	entry->name = stateName;
	entry->state = std::move(state);
	_latestState = entry.get();
	_states.insert({stateName, std::move(entry)});
	return *this;
}

FsmBuilder& FsmBuilder::on(std::string_view transitionName) {
	_latestOnTransition.clear();
	if (!_latestState) {
		throw TACTICS_EXCEPTION("Can't create a transition with name [{}]. No state has been setup in the FsmBuilder",
			transitionName);
	}

	if (_latestState->transitions.contains(transitionName)) {
		throw TACTICS_EXCEPTION("Can't create a transition with the same name [{}]. State [{}]",
			transitionName, _latestState->name);
	}

	_latestState->transitions.insert({transitionName, {}});
	_latestOnTransition = transitionName;
	return *this;
}

FsmBuilder& FsmBuilder::onAppExitRequest() {
	return on(FsmEventListener::appExitRequestTransition);
}

FsmBuilder& FsmBuilder::jumpTo(std::string_view targetState) {
	return jumpTo(nullptr, targetState);
}

FsmBuilder& FsmBuilder::exitFsm() {
	return jumpTo(nullptr, Fsm::exitState);
}

FsmBuilder& FsmBuilder::jumpTo(std::function<bool()> condition, std::string_view targetState) {
	if (!_latestState) {
		throw TACTICS_EXCEPTION("Can't add a target state to jumpTo. No state has been setup in the FsmBuilder. TargetState [{}]",
			targetState);
	}

	if (_latestOnTransition.empty()) {
		throw TACTICS_EXCEPTION("Can't add a target state to jumpTo. No 'on' event has been defined yet. State [{}], TargetState [{}]",
			_latestState->name, targetState);
	}

	_latestState->transitions[_latestOnTransition].push_back(FsmTransitionTarget{std::string(targetState), condition});
	return *this;
}

std::tuple<std::unique_ptr<Fsm>, std::unique_ptr<FsmInfo>> FsmBuilder::build(std::string_view startStateName, FsmExternalController* externalController) {
	if (!_states.contains(startStateName)) {
		throw TACTICS_EXCEPTION("Can't build FSM. Start state [{}] is not valid", startStateName);
	} else if (_states.empty()) {
		throw TACTICS_EXCEPTION("Can't build FSM. No states have been added to the FSM");
	}

	auto fsmInfo = std::make_unique<FsmInfo>();
	fsmInfo->startState = startStateName;
	for (auto& [name, state] : _states) {
		fsmInfo->states.emplace_back(name, state->transitions);
	}

	auto fsm = std::make_unique<Fsm>(std::move(_states), startStateName, externalController);
	_states.clear();
	_latestOnTransition.clear();

	return {std::move(fsm), std::move(fsmInfo)};
}

}
