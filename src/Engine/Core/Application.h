#pragma once

#include <Libs/Fsm/FsmBuilder.h>
#include <Libs/Utility/Service/ServiceLocator.h>

#include <memory>

namespace tactics {

class Application {
public:
	virtual ~Application() = default;

	/**
	 * Invoked after the engine has initialized all subsystems, which are now accessible via the service locator.
	 * This function is where the application should construct the FSM using the provided FSM builder and return
	 * the name of the state that will serve as the initial state.
	 *
	 * @param serviceLocator Provides access to initialized subsystems.
	 * @param fsmBuilder A builder for constructing the finite state machine.
	 * @return The name of the initial state.
	 */
	virtual std::string initialize(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) = 0;
};

}
