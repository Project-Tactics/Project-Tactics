#include "MapState.h"

#include <Libs/Overlay/OverlaySystem.h>
#include <Libs/Overlay/ExampleOverlay.h>
#include <Engine/Rendering/RenderSystem.h>

namespace tactics {

MapState::MapState(RenderSystem& renderSystem, OverlaySystem& overlaySystem)
	: _renderSystem(renderSystem)
	, _overlaySystem(overlaySystem) {

}

FsmAction MapState::enter() {
	_exitNextFrame = false;

	_overlaySystem.addOverlay<ExampleOverlay>("ImGuiDemo");

	return FsmAction::none();
}

void MapState::exit() {
	_overlaySystem.removeOverlay("ImGuiDemo");
}

FsmAction MapState::update() {
	if (_exitNextFrame) {
		return FsmAction::transition("exit");
	}

	_renderSystem.drawSomething();

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
