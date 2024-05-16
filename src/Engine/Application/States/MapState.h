#pragma once

#include <Libs/Fsm/FsmState.h>

namespace tactics {

class OverlaySystem;
class RenderSystem;

class MapState: public FsmState {
public:
	MapState(RenderSystem& renderSystem, OverlaySystem& overlaySystem);
	FsmAction update() override;
	FsmAction enter() override;
	void exit() override;

private:
	bool onKeyPress(SDL_KeyboardEvent& event) override;

	bool _exitNextFrame{};
	RenderSystem& _renderSystem;
	OverlaySystem& _overlaySystem;
};

}
