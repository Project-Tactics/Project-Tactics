#include "MapState.h"

#include "../Component/RotateAroundPoint.h"

#include <Engine/Scene/SceneSystem.h>

#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Ecs/Component/FrustumComponent.h>

namespace tactics {

MapState::MapState(ServiceLocator& serviceLocator, unsigned int mapIndex): FsmStateWithServices(serviceLocator), _mapIndex(mapIndex) {
}

FsmAction MapState::enter() {
	auto& sceneSystem = getService<SceneSystem>();
	auto mapName = fmt::format("map{:02d}", _mapIndex);
	sceneSystem.createEntity("map", mapName);

	return FsmAction::none();
}

FsmAction MapState::update() {
	auto& scene = getService<SceneSystem>();
	component::RotateAroundPointSystem::update(scene.getRegistry().view<component::Transform, component::RotateAroundPoint>());

	return FsmAction::none();
}

void MapState::exit() {
	auto& sceneSystem = getService<SceneSystem>();
	sceneSystem.clearScene();
}

FsmEventAction MapState::onKeyPress(SDL_KeyboardEvent& event) {
	if (event.keysym.scancode == SDL_SCANCODE_ESCAPE) {
		return FsmEventAction::transition("exit");
	} else if (event.keysym.scancode == SDL_SCANCODE_RETURN) {
		return FsmEventAction::transition("next");
	} else if (event.keysym.scancode == SDL_SCANCODE_SPACE) {
		return FsmEventAction::transition("empty");
	}

	return FsmEventAction::none();
}

}
