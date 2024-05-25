#pragma once

#include <Libs/Fsm/FsmStateWithServices.h>

namespace tactics {

class OverlaySystem;
class RenderSystem;

class DemoState: public FsmStateWithServices {
public:
	using FsmStateWithServices::FsmStateWithServices;
	FsmAction update() override;
	FsmAction enter() override;
	void exit() override;

private:
	bool onKeyPress(SDL_KeyboardEvent& event) override;
	void _rotateCamera();

	bool _exitNextFrame{};
	float _cameraAngleX{};
};

}
