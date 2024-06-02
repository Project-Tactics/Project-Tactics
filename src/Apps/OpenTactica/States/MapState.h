#pragma once

#include <Libs/Fsm/FsmStateWithServices.h>

#include <Engine/Scene/SceneSystem.h>

namespace tactics {

class MapState: public FsmStateWithServices {
public:
	MapState(ServiceLocator& serviceLocator, unsigned int mapIndex);
	FsmAction enter() override;
	FsmAction update() override;
	void exit() override;

private:
	bool onKeyPress(SDL_KeyboardEvent& event) override;

	bool _exitNextFrame{};
	bool _exitNextFrameWithNextTransition{};
	unsigned int _mapIndex{};
};

}
