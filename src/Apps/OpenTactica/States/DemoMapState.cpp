#include "DemoMapState.h"

#include "../Component/RotateAroundPoint.h"
#include "../Component/RotateItem.h"

#include <Engine/Scene/SceneSystem.h>

#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Ecs/Component/FrustumComponent.h>

namespace tactics {

FsmAction DemoMapState::enter() {
	auto& sceneSystem = getService<SceneSystem>();
	auto mapName = fmt::format("map{:02d}", _mapIndex);
	sceneSystem.createEntity("map", mapName);

	return FsmAction::none();
}

FsmAction DemoMapState::update() {
	auto& scene = getService<SceneSystem>();
	component::RotateAroundPointSystem::update(scene.getRegistry().view<component::Transform, component::RotateAroundPoint>());
	component::RotateItemSystem::update(scene.getRegistry().view<component::Transform, component::RotateItem>());

	return FsmAction::none();
}

void DemoMapState::exit() {
	auto& sceneSystem = getService<SceneSystem>();
	sceneSystem.clearScene();
}

FsmEventAction DemoMapState::onKeyPress(SDL_KeyboardEvent& event) {
	if (event.keysym.scancode == SDL_SCANCODE_ESCAPE) {
		return FsmEventAction::transition("exit");
	} else if (event.keysym.scancode == SDL_SCANCODE_RETURN) {
		auto& sceneSystem = getService<SceneSystem>();
		sceneSystem.clearScene();
		_mapIndex = (_mapIndex + 1) % 5;
		auto mapName = fmt::format("map{:02d}", _mapIndex);
		sceneSystem.createEntity("map", mapName);
	}

	return FsmEventAction::none();
}

}
