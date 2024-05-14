#pragma once

#include <Libs/Fsm/FsmState.h>

namespace tactics {

class MapState: public FsmState {
public:
	FsmAction update() override;
	FsmAction enter() override;
	void exit() override;

private:
	bool onKeyPress(SDL_KeyboardEvent& event) override;

	bool _exitNextFrame{};
};

}
