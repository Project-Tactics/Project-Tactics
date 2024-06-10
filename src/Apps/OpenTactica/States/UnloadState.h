#pragma once

#include <Libs/Fsm/FsmStateWithServices.h>

namespace tactics {

class UnloadState: public FsmStateWithServices {
public:
	UnloadState(ServiceLocator& services, const std::string& packageName);
	FsmAction enter() override;
	FsmAction update() override;
	void exit() override;

private:
	std::string _packageName;
};

}
