#pragma once

#include <Libs/Fsm/FsmState.h>

namespace tactics {

class RenderSystem;
class ResourceSystem;
class OverlaySystem;

class StartState: public FsmState {
public:
	StartState(ResourceSystem& resourceSystem, RenderSystem& renderSystem, OverlaySystem& overlaySystem);

	FsmAction enter() override;
	FsmAction update() override;
	void exit() override;

private:
	ResourceSystem& _resourceSystem;
	RenderSystem& _renderSystem;
	OverlaySystem& _overlaySystem;
};

}
