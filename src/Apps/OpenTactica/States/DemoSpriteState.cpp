#include "DemoSpriteState.h"

#include <Engine/Scene/SceneSystem.h>
#include <Libs/Ecs/Component/TransformComponent.h>

#include "../Component/RotateAroundPoint.h"

namespace tactics {

FsmAction DemoSpriteState::enter() {
	auto& sceneSystem = getService<SceneSystem>();
	int edge = 2;
	for (auto x = -edge / 2; x < edge / 2; x++) {
		for (auto y = -edge / 2; y < edge / 2; y++) {
			auto entity = sceneSystem.createEntity("char", "character");
			auto& transform = entity.getComponent<component::Transform>();
			transform.setPosition(glm::vec3(x * 1.0f, y * 1.0f, 0.0f));
		}
	}
	return FsmAction::none();
}

void DemoSpriteState::exit() {
	auto& sceneSystem = getService<SceneSystem>();
	sceneSystem.clearScene();
}

FsmAction DemoSpriteState::update() {
	component::RotateAroundPointSystem::update(getService<SceneSystem>().getRegistry().view<component::Transform, component::RotateAroundPoint>());
	return FsmAction::none();
}

FsmEventAction DemoSpriteState::onKeyPress(SDL_KeyboardEvent& event) {
	if (event.keysym.sym == SDLK_ESCAPE) {
		return FsmEventAction::transition("exit");
	}
	return FsmEventAction::none();
}

}
