#pragma once

#include <Libs/Fsm/FsmStateWithServices.h>

namespace tactics {

class DemoSpriteState: public FsmStateWithServices {
public:
	using FsmStateWithServices::FsmStateWithServices;
	FsmAction update() override;
	FsmAction enter() override;
	void exit() override;

private:
	FsmEventAction onKeyPress(SDL_KeyboardEvent& event) override;
};

}
