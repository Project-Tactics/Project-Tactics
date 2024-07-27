#pragma once

#include "FsmTypes.h"

#include <Libs/Event/EventsListener.h>

#include <memory>
#include <string>

namespace tactics {

class FsmExternalController;

class Fsm {
public:
	Fsm(FsmStateEntries states, const HashId& startStateName, FsmExternalController* externalController);

	/**
	 * @brief Updates the current state of the Fsm and executes any transition.
	 */
	void update();

	/*
	 * @brief Check if the Fsm has reached its internal Fsm::exitState.
	 */
	[[nodiscard]] bool hasReachedExitState() const;

	/**
	 * Global string whish specify the state used to exit from the Fsm.
	 * When the Fsm jumps to this state, Fsm::hasReachedExitState() will return true.
	 */
	static const HashId exitState;

private:
	void _goToState(const HashId& stateName);
	void _executeTransition(const HashId& transition);
	[[nodiscard]] FsmStateEntry* _getStateByName(const HashId& stateName);
	bool _performExternalUpdateTransition();

	template<typename TAction> void _performAction(const TAction& action) {
		if (action.hasTransition()) {
			_executeTransition(action.transitionName());
		}
	}

	FsmStateEntries _states;
	FsmStateEntry* _currentState{};

	/**
	 * The External Controller is mostly meant to control the transition of the fsm by requesting them during the
	 * update() method. Its main purpose is to allow debug or external control of the Fsm. It doesn't replace
	 * the Fsm/FsmState logic and shouldn't be used for domain logic reason.
	 */
	FsmExternalController* _externalController{};

	bool _hasReachedExitState{};
	HashId _startStateName;
	HashId _scheduledTransition;
};

} // namespace tactics
