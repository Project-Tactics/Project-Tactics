#pragma once

#include <Libs/Fsm/FsmStateWithServices.h>

#include <Engine/Scene/SceneSystem.h>

namespace tactics {

class MapState: public FsmStateWithServices {
public:
	using FsmStateWithServices::FsmStateWithServices;
	FsmAction enter() override;
	FsmAction update() override;
	void exit() override;

private:
	bool onKeyPress(SDL_KeyboardEvent& event) override;

	bool _exitNextFrame{};
};

}
