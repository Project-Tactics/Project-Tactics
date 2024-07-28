#pragma once

#include <Libs/Fsm/FsmStateWithServices.h>

namespace tactics {

class SampleSelectionState : public FsmStateWithServices {
public:
	using FsmStateWithServices::FsmStateWithServices;
	FsmAction enter() override;
	void exit() override;
	FsmAction update() override;
};

} // namespace tactics
