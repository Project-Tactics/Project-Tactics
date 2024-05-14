#pragma once

#include <Libs/Fsm/FsmState.h>

namespace tactics {

class StartState: public FsmState {
public:
	FsmAction update() override;
	FsmAction enter() override;
	void exit() override;
};

}
