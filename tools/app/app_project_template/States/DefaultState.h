#pragma once

#include <Libs/Fsm/FsmStateWithServices.h>

namespace tactics {

class ##APP_NAME##State : public FsmStateWithServices {
public:
	using FsmStateWithServices::FsmStateWithServices;
	FsmAction enter() override;
	FsmAction update() override;
	void exit() override;
};

} // namespace tactics
