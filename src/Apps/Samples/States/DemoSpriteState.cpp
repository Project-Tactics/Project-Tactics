#include "DemoSpriteState.h"

#include "../Component/RotateAroundPoint.h"

#include <Engine/Scene/SceneSystem.h>

#include <Libs/Ecs/Component/TransformComponent.h>

namespace tactics {

FsmAction DemoSpriteState::enter() {
	_createCharacters({0, 0, 0}, 1);
	_createCharacters({0, 0, -0.5}, 1);
	_createCharacters({0, 0, 0.5}, 1);
	return FsmAction::none();
}

void DemoSpriteState::_createCharacters(const glm::vec3& offset, int count) {
	auto& sceneSystem = getService<SceneSystem>();
	for (auto x = 0; x < count; x++) {
		for (auto y = 0; y < count; y++) {
			auto entity = sceneSystem.createEntity("char"_id, "character"_id);
			auto& transform = entity.getComponent<component::Transform>();
			auto position = glm::vec3(static_cast<float>(x) * 1.0f, static_cast<float>(y) * 1.0f, 0.0f);
			position += offset;
			transform.setPosition(position);
		}
	}
}

void DemoSpriteState::exit() {
	auto& sceneSystem = getService<SceneSystem>();
	sceneSystem.clearScene();
}

FsmAction DemoSpriteState::update() {
	component::RotateAroundPointSystem::update(getService<SceneSystem>().getRegistry());
	return FsmAction::none();
}

FsmEventAction DemoSpriteState::onKeyPress(SDL_KeyboardEvent& event) {
	if (event.keysym.sym == SDLK_ESCAPE) {
		return FsmEventAction::transition("exit"_id);
	} else if (event.keysym.sym == SDLK_1) {
		auto& sceneSystem = getService<SceneSystem>();
		sceneSystem.clearScene();
		_createCharacters({}, 2);
		_createCharacters({0.15, 0, -2}, 2);
		_createCharacters({-1, 0, 2}, 2);
	} else if (event.keysym.sym == SDLK_2) {
		auto& sceneSystem = getService<SceneSystem>();
		sceneSystem.clearScene();
		_createCharacters({}, 2);
		_createCharacters({-1, 0, 2}, 2);
	} else if (event.keysym.sym == SDLK_3) {
		auto& sceneSystem = getService<SceneSystem>();
		sceneSystem.clearScene();
		_createCharacters({0.15, 0, -2}, 2);
		_createCharacters({-1, 0, 2}, 2);
	}
	return FsmEventAction::none();
}

} // namespace tactics
