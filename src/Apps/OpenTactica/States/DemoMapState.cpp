#include "DemoMapState.h"

#include "../Component/BattleCamera.h"
#include "../Component/CharacterFacing.h"
#include "../Component/RotateAroundPoint.h"
#include "../Component/RotateItem.h"

#include <Engine/Scene/SceneSystem.h>

#include <Libs/Ecs/Component/FrustumComponent.h>
#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Utility/String/Math.h>

namespace tactics {

FsmAction DemoMapState::enter() {
	_createScene();
	return FsmAction::none();
}

FsmAction DemoMapState::update() {
	using namespace component;
	auto& registry = getService<SceneSystem>().getRegistry();
	RotateAroundPointSystem::update(registry);
	RotateItemSystem::update(registry);
	CharacterFacingSystem::update(registry);
	BattleCameraSystem::update(registry);
	return FsmAction::none();
}

void DemoMapState::exit() {
	auto& sceneSystem = getService<SceneSystem>();
	sceneSystem.clearScene();
}

FsmEventAction DemoMapState::onKeyPress(SDL_KeyboardEvent& event) {
	if (event.keysym.scancode == SDL_SCANCODE_ESCAPE) {
		return FsmEventAction::transition("exit"_id);
	} else if (event.keysym.scancode == SDL_SCANCODE_RETURN) {
		_mapIndex = (_mapIndex + 1) % 5;
		_createScene();
	} else if (event.keysym.scancode == SDL_SCANCODE_LEFT) {
		auto& sceneSystem = getService<SceneSystem>();
		auto& battleCamera = sceneSystem.getCurrentCamera().getComponent<component::BattleCamera>();
		battleCamera.rotateToNextStep();
	} else if (event.keysym.scancode == SDL_SCANCODE_RIGHT) {
		auto& sceneSystem = getService<SceneSystem>();
		auto& battleCamera = sceneSystem.getCurrentCamera().getComponent<component::BattleCamera>();
		battleCamera.rotateToPrevStep();
	}

	return FsmEventAction::none();
}

void DemoMapState::_createScene() {
	using namespace component;

	auto& sceneSystem = getService<SceneSystem>();
	sceneSystem.clearScene();
	auto mapName = fmt::format("map{:02d}", _mapIndex);
	sceneSystem.createEntity("map"_id, HashId(mapName));
	sceneSystem.createEntity("char"_id, "character"_id);
	sceneSystem.createEntity("shadow"_id, "charShadow"_id);

	struct CharLayout {
		glm::vec3 translate;
		Facing facing;
	};

	auto positions = std::array{CharLayout{{-0.56f, 0, -0.56f}, Facing::East},
								CharLayout{{-1.12f, 0, -0.56f}, Facing::West},
								CharLayout{{-0.56f, 0, 2.80f}, Facing::South},
								CharLayout{{-1.12f, 0, 2.80f}, Facing::North}};

	for (auto [pos, facing] : positions) {
		auto character = sceneSystem.createEntity("char"_id, "character"_id);
		auto& charTransform = character.getComponent<Transform>();
		charTransform.translate(pos);
		character.getComponent<CharacterFacing>().facing = facing;
		auto shadow = sceneSystem.createEntity("shadow"_id, "charShadow"_id);
		auto& shadowTransform = shadow.getComponent<Transform>();
		shadowTransform.translate(pos);
	}
}

} // namespace tactics
