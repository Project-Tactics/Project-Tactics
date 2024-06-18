#pragma once

#include "FsmTypes.h"

#include <memory>
#include <string>
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
	/*
	* Special transition triggered when the app is requesting to exit.
	* Most of the time as a result of clicking the X button on the window.
	* This is just a more verbose way to define the manual transition on(_appExitRequestTransition)
	*/
	FsmBuilder& onAppExitRequest();

	FsmBuilder& exitFsm();
	FsmBuilder& jumpTo(std::string_view targetState);
	FsmBuilder& jumpTo(std::function<bool()> condition, std::string_view targetState);

	std::tuple<std::unique_ptr<Fsm>, std::unique_ptr<FsmInfo>> build(std::string_view startStateName, FsmExternalController* externalController);

private:
	FsmStateEntries _states;
	FsmStateEntry* _latestState{};
	std::string _latestOnTransition;
};

}
