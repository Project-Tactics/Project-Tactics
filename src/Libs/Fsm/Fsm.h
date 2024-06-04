#pragma once

#include <Libs/Event/EventsListener.h>

#include "FsmTypes.h"

#include <string>
#include <memory>

namespace tactics {

class FsmExternalController;

class Fsm: public EventsListener {
public:
	Fsm(FsmStateEntries states, std::string_view startStateName, FsmExternalController* externalController);

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
	static const std::string_view exitState;

private:
	// FsmEventHandler implementation
	[[nodiscard]] bool onEvent(SDL_Event& event) override;

	void _goToState(std::string_view stateName);
	void _performAction(FsmAction& action);
	void _executeTransition(std::string_view transition);
	[[nodiscard]] FsmStateEntry* _getStateByName(std::string_view stateName);
	bool _performExternalUpdateTransition();

	template<typename TAction>
	void _performAction(const TAction& action) {
		if (action.hasTransition()) {
			_executeTransition(action.transitionName());
		}
	}

	FsmStateEntries _states;
	FsmStateEntry* _currentState{};

	/**
	 * The External Controller is mostly meant to control the transition of the fsm by requesting them during the update() method
	 * Its main purpose is to allow debug or external control of the Fsm in dev mode.
	 * It doesn't replace the Fsm/FsmState logic and shouldn't be used for domain logic reason.
	 */
	FsmExternalController* _externalController{};

	bool _hasReachedExitState{};
	std::string _startStateName;
};

}
