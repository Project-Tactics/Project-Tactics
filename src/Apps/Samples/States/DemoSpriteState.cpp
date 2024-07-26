#include "DemoSpriteState.h"

#include "../Component/RotateAroundPoint.h"

#include <Engine/Scene/SceneSystem.h>

#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Input/InputSystem.h>

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

	auto& inputSystem = getService<InputSystem>();
	if (inputSystem.checkAction("exitFromState")) {
		return FsmAction::transition("exit"_id);
	}

	return FsmAction::none();
}

} // namespace tactics
