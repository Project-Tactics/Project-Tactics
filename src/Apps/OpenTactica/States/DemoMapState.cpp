#include "DemoMapState.h"

#include "../Component/BattleCamera.h"
#include "../Component/CharacterFacing.h"
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

	sceneSystem.createEntity("char", "character");

	return FsmAction::none();
}

FsmAction DemoMapState::update() {
	using namespace component;
	auto& scene = getService<SceneSystem>();
	component::RotateAroundPointSystem::update(scene.getRegistry().view<Transform, RotateAroundPoint>());
	component::RotateItemSystem::update(scene.getRegistry().view<Transform, RotateItem>());

	auto cameraView = scene.getRegistry().view<Transform, Camera, CurrentCamera>();
	component::CharacterFacingSystem::update(cameraView, scene.getRegistry().view<CharacterFacing, SpriteAnimation, Sprite>());

	component::BattleCameraSystem::update(scene.getRegistry().view<BattleCamera, Transform>());

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
