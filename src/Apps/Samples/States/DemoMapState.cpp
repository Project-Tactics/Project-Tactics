#include "DemoMapState.h"

#include "../Component/BattleCamera.h"
#include "../Component/CharacterFacing.h"
#include "../Component/RotateAroundPoint.h"
#include "../Component/RotateItem.h"

#include <Engine/Scene/SceneSystem.h>

#include <Libs/Ecs/Component/FrustumComponent.h>
#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Input/InputSystem.h>

namespace tactics {

FsmAction DemoMapState::enter() {
	_createScene();
	_setupInput();
	return FsmAction::none();
}

FsmAction DemoMapState::update() {
	using namespace component;
	auto& registry = getService<SceneSystem>().getRegistry();
	RotateAroundPointSystem::update(registry);
	RotateItemSystem::update(registry);
	CharacterFacingSystem::update(registry);
	BattleCameraSystem::update(registry);

	auto& inputSystem = getService<InputSystem>();
	if (inputSystem.checkAction("exitFromState")) {
		return FsmAction::transition("exit"_id);
	} else if (inputSystem.checkAction("loadNextMap")) {
		_mapIndex = (_mapIndex + 1) % 5;
		_createScene();
	}

	return FsmAction::none();
}

void DemoMapState::exit() {
	auto& sceneSystem = getService<SceneSystem>();
	sceneSystem.clearScene();

	component::BattleCameraSystem::uninit(sceneSystem.getRegistry());
}

void DemoMapState::_createScene() {
	using namespace component;

	auto& sceneSystem = getService<SceneSystem>();
	BattleCameraSystem::init(sceneSystem.getRegistry());

	_createCamera("mapCamera"_id);

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

void DemoMapState::_setupInput() {
	auto& inputSystem = getService<InputSystem>();
	inputSystem.assignInputMap("mapDemoInput");
}

} // namespace tactics
