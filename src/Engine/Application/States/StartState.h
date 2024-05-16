#pragma once

#include <Libs/Fsm/FsmState.h>

namespace tactics {

class RenderSystem;
class OverlaySystem;

class StartState: public FsmState {
public:
	StartState(RenderSystem& renderSystem, OverlaySystem& overlaySystem);

	FsmAction enter() override;
	FsmAction update() override;
	void exit() override;

private:
	RenderSystem& _renderSystem;
	OverlaySystem& _overlaySystem;
};

}
