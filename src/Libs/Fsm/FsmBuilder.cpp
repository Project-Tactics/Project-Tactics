#include "FsmBuilder.h"

#include "Fsm.h"

#include <Libs/Utility/Exception.h>

namespace tactics {

const HashId FsmBuilder::appExitRequestTransition = "_appExitRequest"_id;

FsmBuilder& FsmBuilder::state(std::string_view stateName, std::unique_ptr<FsmState> state) {
	auto hashedStateName = HashId(stateName);
	if (_states.contains(hashedStateName)) {
		throw TACTICS_EXCEPTION("Can't add state with name [{}]. State already exists", stateName);
	}
	_latestOnTransition = HashId::none;
	auto entry = std::make_unique<FsmStateEntry>();
	entry->name = hashedStateName;
	entry->state = std::move(state);
	_latestState = entry.get();
	_states.insert({hashedStateName, std::move(entry)});
	return *this;
}

FsmBuilder& FsmBuilder::on(std::string_view transitionName) {
	return _on(HashId(transitionName));
}

FsmBuilder& FsmBuilder::_on(const HashId& transitionName) {
	_latestOnTransition = HashId::none;
	if (!_latestState) {
		throw TACTICS_EXCEPTION("Can't create a transition with name [{}]. No state has been setup in the FsmBuilder",
								transitionName);
	}

	if (_latestState->transitions.contains(transitionName)) {
		throw TACTICS_EXCEPTION("Can't create a transition with the same name [{}]. State [{}]",
								transitionName,
								_latestState->name);
	}

	_latestState->transitions.insert({transitionName, {}});
	_latestOnTransition = transitionName;
	return *this;
}

FsmBuilder& FsmBuilder::onAppExitRequest() {
	return _on(appExitRequestTransition);
}

FsmBuilder& FsmBuilder::jumpTo(std::string_view targetState) {
	return jumpTo(nullptr, targetState);
}

FsmBuilder& FsmBuilder::exitFsm() {
	return _jumpTo(nullptr, Fsm::exitState);
}

FsmBuilder& FsmBuilder::_jumpTo(std::function<bool()> condition, const HashId& targetState) {
	if (!_latestState) {
		throw TACTICS_EXCEPTION(
			"Can't add a target state to jumpTo. No state has been setup in the FsmBuilder. TargetState [{}]",
			targetState);
	}

	if (_latestOnTransition.isEmpty()) {
		throw TACTICS_EXCEPTION(
			"Can't add a target state to jumpTo. No 'on' event has been defined yet. State [{}], TargetState [{}]",
			_latestState->name,
			targetState);
	}

	_latestState->transitions[_latestOnTransition].push_back(FsmTransitionTarget{targetState, condition});
	return *this;
}

FsmBuilder& FsmBuilder::jumpTo(std::function<bool()> condition, std::string_view targetState) {
	return _jumpTo(condition, HashId(targetState));
}

std::tuple<std::unique_ptr<Fsm>, std::unique_ptr<FsmInfo>>
FsmBuilder::build(const HashId& startStateName, FsmExternalController* externalController) {
	if (!_states.contains(startStateName)) {
		throw TACTICS_EXCEPTION("Can't build FSM. Start state [{}] is not valid", startStateName);
	} else if (_states.empty()) {
		throw TACTICS_EXCEPTION("Can't build FSM. No states have been added to the FSM");
	}

	auto fsmInfo = std::make_unique<FsmInfo>();
	fsmInfo->startState = startStateName;
	for (auto& [name, state] : _states) {
		fsmInfo->states.emplace_back(state->name, state->transitions);
	}

	auto fsm = std::make_unique<Fsm>(std::move(_states), startStateName, externalController);
	_states.clear();
	_latestOnTransition = HashId::none;

	return {std::move(fsm), std::move(fsmInfo)};
}

} // namespace tactics
