#pragma once

#include "FsmTypes.h"

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <utility>

#include "Fsm.h"

/*
* @brief Helper class used to build an Fsm in a more concise way
*
* Example:
*     FsmBuilder builder;
*     builder
*         .state<MainMenuState>("MainMenu")
*             .on("options").jumpTo("Options")
*
*         .state<OptionsState>("Options")
*             .on("exitButton").exitFsm()
*             .on("back").jumpTo("MainMenu")
*/

namespace tactics {

class FsmBuilder {
public:
	template<typename TState, typename ...TArgs>
	FsmBuilder& state(std::string_view stateName, TArgs &&... args) {
		auto stateInstace = std::make_unique<TState>(std::forward<TArgs>(args) ...);
		state(stateName, std::move(stateInstace));
		return *this;
	}

	FsmBuilder& state(std::string_view stateName, std::unique_ptr<FsmState> state);
	FsmBuilder& on(std::string_view transitionName);

	FsmBuilder& exitFsm();
	FsmBuilder& jumpTo(std::string_view targetState);
	FsmBuilder& jumpTo(std::function<bool()> condition, std::string_view targetState);

	std::unique_ptr<Fsm> build(std::string_view startStateName);

private:
	FsmStateEntries _states;
	FsmStateEntry* _latestState{};
	std::string _latestOnTransition;
};

}
