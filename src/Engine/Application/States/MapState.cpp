#include "MapState.h"

#include "../Overlay/DebugOverlay.h"

#include <Libs/Overlay/OverlaySystem.h>
#include <Libs/Overlay/ExampleOverlay.h>
#include <Libs/Overlay/MainOverlay.h>
#include <Engine/Rendering/RenderSystem.h>

namespace tactics {

MapState::MapState(RenderSystem& renderSystem, OverlaySystem& overlaySystem)
	: _renderSystem(renderSystem)
	, _overlaySystem(overlaySystem) {

}

FsmAction MapState::enter() {
	_exitNextFrame = false;

	_overlaySystem.addOverlay<MainOverlay>("Main", true, _overlaySystem);
	_overlaySystem.addOverlay<DebugOverlay>("Debug", false, _renderSystem);
	_overlaySystem.addOverlay<ExampleOverlay>("Example", false);

	return FsmAction::none();
}

void MapState::exit() {
	_overlaySystem.removeOverlay("Main");
	_overlaySystem.removeOverlay("Debug");
	_overlaySystem.removeOverlay("Example");
}

FsmAction MapState::update() {
	if (_exitNextFrame) {
		return FsmAction::transition("exit");
	}

	return FsmAction::none();
}

bool MapState::onKeyPress(SDL_KeyboardEvent& event) {
	if (event.keysym.scancode == SDL_Scancode::SDL_SCANCODE_ESCAPE) {
		_exitNextFrame = true;
		return true;
	}

	return false;
}

}
