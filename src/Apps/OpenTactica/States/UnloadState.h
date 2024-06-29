#pragma once

#include <Libs/Fsm/FsmStateWithServices.h>

#include <Libs/Utility/HashString.h>

namespace tactics {

class UnloadState: public FsmStateWithServices {
public:
	UnloadState(ServiceLocator& services, const hash_string& packageName);
	FsmAction enter() override;
	FsmAction update() override;
	void exit() override;

private:
	hash_string _packageName;
};

}
