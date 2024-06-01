#pragma once

#include <Libs/Fsm/FsmStateWithServices.h>
#include <Libs/Ecs/Entity.h>

#include <glm/glm.hpp>

namespace tactics {

class DemoState: public FsmStateWithServices {
public:
	using FsmStateWithServices::FsmStateWithServices;
	FsmAction update() override;
	FsmAction enter() override;
	void exit() override;

private:
	bool onKeyPress(SDL_KeyboardEvent& event) override;

	bool _exitNextFrame{};
	bool _exitNextFrameAlt{};
	float _cameraAngleX{};
};

}
