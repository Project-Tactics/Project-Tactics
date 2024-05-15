#include "MapState.h"

#include <Engine/Rendering/RenderSystem.h>

namespace tactics {

MapState::MapState(RenderSystem& renderSystem): _renderSystem(renderSystem) {

}

FsmAction MapState::enter() {
	_exitNextFrame = false;
	return FsmAction::none();
}

void MapState::exit() {
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
