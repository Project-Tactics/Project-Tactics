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
	_createScene();
	return FsmAction::none();
}

FsmAction DemoMapState::update() {
	using namespace component;
	auto& scene = getService<SceneSystem>();
	component::RotateAroundPointSystem::update(scene.getRegistry().view<Transform, RotateAroundPoint>());
	component::RotateItemSystem::update(scene.getRegistry().view<Transform, RotateItem>());

	auto [battleCameraEntity, battleCamera] = *scene.getRegistry().view<BattleCamera>().each().begin();
	component::CharacterFacingSystem::update(battleCamera, scene.getRegistry().view<CharacterFacing, SpriteAnimation, Sprite>());

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
		_mapIndex = (_mapIndex + 1) % 5;
		_createScene();
	} else if (event.keysym.scancode == SDL_SCANCODE_LEFT) {
		auto& sceneSystem = getService<SceneSystem>();
		auto& battleCamera = sceneSystem.getCurrentCamera().getComponent<component::BattleCamera>();
		battleCamera.nextStep = (battleCamera.nextStep + 1) % battleCamera.rotationSteps.size();
	} else if (event.keysym.scancode == SDL_SCANCODE_RIGHT) {
		auto& sceneSystem = getService<SceneSystem>();
		auto& battleCamera = sceneSystem.getCurrentCamera().getComponent<component::BattleCamera>();
		battleCamera.nextStep = (battleCamera.nextStep - 1) % battleCamera.rotationSteps.size();
	}

	return FsmEventAction::none();
}

void DemoMapState::_createScene() {
	using namespace component;

	auto& sceneSystem = getService<SceneSystem>();
	sceneSystem.clearScene();
	auto mapName = fmt::format("map{:02d}", _mapIndex);
	sceneSystem.createEntity("map", mapName);
	sceneSystem.createEntity("char", "character");
	sceneSystem.createEntity("shadow", "charShadow");

	struct CharLayout {
		glm::vec3 translate;
		Facing facing;
	};

	auto positions = std::array{
		CharLayout{{-0.56f, 0, -0.56f}, Facing::West},
		CharLayout{{-1.12f, 0, -0.56f}, Facing::West},
		CharLayout{{-0.56f, 0, 2.80f}, Facing::South},
		CharLayout{{-1.12f, 0, 2.80f}, Facing::North}
	};

	for (auto [pos, facing] : positions) {
		auto character = sceneSystem.createEntity("char", "character");
		auto& charTransform = character.getComponent<Transform>();
		charTransform.translate(pos);
		character.getComponent<CharacterFacing>().facing = facing;
		auto shadow = sceneSystem.createEntity("shadow", "charShadow");
		auto& shadowTransform = shadow.getComponent<Transform>();
		shadowTransform.translate(pos);
	}
}

}
