#pragma once

#include <Libs/Fsm/FsmBuilder.h>
#include <Libs/Utility/Service/ServiceLocator.h>

#include <memory>

namespace tactics {

class Application {
public:
	virtual ~Application() = default;

	/**
	 * Invoked after the engine has initialized all systems, which are now accessible via the service locator.
	 * This function is where the application should construct the FSM using the provided FSM builder and return
	 * the name of the state that will serve as the initial state.
	 *
	 * @param serviceLocator Provides access to initialized systems.
	 * @param fsmBuilder A builder for constructing the finite state machine.
	 * @return The name of the initial state.
	 */
	virtual std::string initialize(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) = 0;

	/*
	* Just a function of convenience to setup the reflection for each custom component which are external to the engine
	* The user can define reflections for components anywhere in the code but this function is a good place to do it and
	* it might be used by the engine in the future to extract debug info useful to be displayed at runtime
	*/
	virtual void setupComponentReflections() = 0;
};

}
