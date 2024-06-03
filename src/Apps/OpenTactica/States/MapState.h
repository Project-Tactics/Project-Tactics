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
	FsmEventAction onKeyPress(SDL_KeyboardEvent& event) override;
	unsigned int _mapIndex{};
};

}
