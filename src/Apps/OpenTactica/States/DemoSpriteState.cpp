#include "DemoSpriteState.h"

#include <Engine/Scene/SceneSystem.h>

namespace tactics {

FsmAction DemoSpriteState::update() {
	return FsmAction::none();
}

FsmAction DemoSpriteState::enter() {
	auto& sceneSystem = getService<SceneSystem>();
	sceneSystem.createEntity("character", "character");
	return FsmAction::none();
}

void DemoSpriteState::exit() {
	auto& sceneSystem = getService<SceneSystem>();
	sceneSystem.clearScene();
}

FsmEventAction DemoSpriteState::onKeyPress(SDL_KeyboardEvent& event) {
	if (event.keysym.sym == SDLK_ESCAPE) {
		return FsmEventAction::transition("exit");
	}
	return FsmEventAction::none();
}

}
