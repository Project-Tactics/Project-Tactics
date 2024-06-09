#pragma once

#include <Libs/Fsm/FsmStateWithServices.h>

namespace tactics {

class DemoState: public FsmStateWithServices {
public:
	using FsmStateWithServices::FsmStateWithServices;
	FsmAction update() override;
	FsmAction enter() override;
	void exit() override;

private:
	FsmEventAction onKeyPress(SDL_KeyboardEvent& event) override;
	void _createPlane();
	void _createTeapot();
	void _createCrate();
	void _createQuads();
	void _createExtraRotatingQuads();
	void _createCustomQuadWithCustomResources();
};

}
