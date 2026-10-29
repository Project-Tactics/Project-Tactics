#include "DemoPongState.h"

#include "../Component/BallMovement.h"
#include "../Component/PlayerMovement.h"
#include "../Component/PongMatch.h"
#include "../Component/Rectangle2DCollider.h"
#include "../Component/RotateItem.h"
#include "../Component/TranslateItem.h"

#include <Engine/Scene/SceneSystem.h>

#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Input/InputSystem.h>

namespace tactics {

FsmAction DemoPongState::enter() {
	using namespace component;

	auto& sceneSystem = getService<SceneSystem>();
	sceneSystem.createEntity("Camera"_id, "defaultCamera"_id);
	sceneSystem.createEntity("background"_id, "background"_id);
	sceneSystem.createEntity("match"_id, "pongMatch"_id);

	auto playerLeft = sceneSystem.createEntity("playerLeft"_id, "player1"_id);
	auto playerRight = sceneSystem.createEntity("playerRight"_id, "player2"_id);
	playerLeft.getComponent<Transform>().setPosition({-4.5f, 0.0f, 0.1f});
	playerRight.getComponent<Transform>().setPosition({4.5f, 0.0f, 0.1f});

	sceneSystem.createEntity("ball"_id, "ball"_id);

	_createScoreDisplay("scoreLeft", PlayerSide::Left, -3.4f);
	_createScoreDisplay("scoreRight", PlayerSide::Right, 3.4f);

	auto& inputSystem = getService<InputSystem>();
	inputSystem.assignInputMap("pongMapping");

	PongMatchSystem::restartMatch(sceneSystem.getRegistry());

	return FsmAction::none();
}

void DemoPongState::exit() {
	auto& inputSystem = getService<InputSystem>();
	inputSystem.unassignInputMap("pongMapping");

	auto& sceneSystem = getService<SceneSystem>();
	sceneSystem.clearScene();
}

FsmAction DemoPongState::update() {
	using namespace component;

	auto& inputSystem = getService<InputSystem>();
	if (inputSystem.checkAction("exitFromState")) {
		return FsmAction::transition("exit"_id);
	}

	auto& registry = getService<SceneSystem>().getRegistry();
	if (PongMatchSystem::isGameOver(registry) && inputSystem.checkAction("restartMatch")) {
		PongMatchSystem::restartMatch(registry);
	}

	PlayerMovementSystem::update(registry);
	TranslateItemSystem::update(registry.view<Transform, TranslateItem>());
	RotateItemSystem::update(registry);
	Rectangle2DColliderSystem::update(registry.view<Transform, Rectangle2DCollider>());
	BallMovementSystem::update(registry);
	PongMatchSystem::update(registry);

	return FsmAction::none();
}

void DemoPongState::_createScoreDisplay(const std::string& name, component::PlayerSide side, float positionX) {
	using namespace component;

	auto& sceneSystem = getService<SceneSystem>();
	auto background = sceneSystem.createEntity(HashId(name + "Bg"), "scorebarBg"_id);
	background.getComponent<Transform>().setPosition({positionX, 2.6f, 0.0f});

	auto score = sceneSystem.createEntity(HashId(name), "scoreNumbers"_id);
	score.getComponent<Transform>().setPosition({positionX, 2.6f, 0.01f});
	score.getComponent<ScoreDisplay>().side = side;
}

} // namespace tactics
