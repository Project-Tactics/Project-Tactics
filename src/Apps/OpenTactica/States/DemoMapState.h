#pragma once

#include <Libs/Fsm/FsmStateWithServices.h>

namespace tactics {

class DemoMapState : public FsmStateWithServices {
public:
	using FsmStateWithServices::FsmStateWithServices;
	FsmAction enter() override;
	FsmAction update() override;
	void exit() override;

private:
	FsmEventAction onKeyPress(SDL_KeyboardEvent& event) override;
	void _createScene();

	unsigned int _mapIndex{};
};

} // namespace tactics
