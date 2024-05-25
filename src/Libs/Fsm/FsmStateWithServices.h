#pragma once

#include "FsmState.h"

#include <Libs/Utility/Service/ServiceLocator.h>

/**
 * @brief A utility abstract class used to store a service locator. It's just containing syntactic sugar to retrieve a service.
 * Injection is still provided by the user who's building the FSM.
 */
namespace tactics {
class ServiceLocator;

class FsmStateWithServices: public FsmState {
public:
	FsmStateWithServices(ServiceLocator& serviceLocator): _serviceLocator(serviceLocator) {}

protected:
	template<typename TService>
	TService& getService() {
		return _serviceLocator.getService<TService>();
	}

private:
	ServiceLocator& _serviceLocator;
};

}
