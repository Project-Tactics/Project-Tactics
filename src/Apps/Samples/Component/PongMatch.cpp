#include "PongMatch.h"

#include "BallMovement.h"
#include "PlayerMovement.h"
#include "RotateItem.h"
#include "TranslateItem.h"

#include <Libs/Ecs/Component/SpriteComponent.h>
#include <Libs/Resource/SpriteSheet/SpriteSheet.h>
#include <Libs/Utility/Log/Log.h>
#include <Libs/Utility/Random.h>
#include <Libs/Utility/Time/EngineTime.h>

#include <algorithm>
#include <cmath>

namespace tactics::component {

void PongMatchSystem::update(entt::registry& registry) {
	auto matchView = registry.view<PongMatch>();
	if (matchView.empty()) {
		return;
	}
	auto& match = matchView.get<PongMatch>(matchView.front());
	match.phaseTime += EngineTime::fixedDeltaTime<float>();

	switch (match.phase) {
	case PongMatchPhase::Serving: {
		if (match.phaseTime >= match.serveDelay) {
			_serveBall(registry, match);
			match.phase = PongMatchPhase::Playing;
			match.phaseTime = 0.0f;
		}
		break;
	}
	case PongMatchPhase::Playing: {
		for (auto [entity, collideWithWall] : registry.view<CollideWithWall>().each()) {
			// The player defending the wall hit by the ball loses the point
			auto scorer = collideWithWall.wallType == WallType::Left ? PlayerSide::Right : PlayerSide::Left;
			_onPointScored(registry, match, scorer);
		}
		registry.clear<CollideWithWall>();
		break;
	}
	case PongMatchPhase::GameOver: {
		// Pulse the score of the winner until the match is restarted
		auto winner = match.scoreLeft >= match.winningScore ? PlayerSide::Left : PlayerSide::Right;
		auto pulse = 1.0f + 0.15f * std::sin(match.phaseTime * 8.0f);
		for (auto [entity, scoreDisplay, transform] : registry.view<ScoreDisplay, Transform>().each()) {
			transform.setScale(scoreDisplay.side == winner ? scoreDisplay.baseScale * pulse : scoreDisplay.baseScale);
		}
		break;
	}
	}
}

void PongMatchSystem::restartMatch(entt::registry& registry) {
	auto matchView = registry.view<PongMatch>();
	if (matchView.empty()) {
		return;
	}
	auto& match = matchView.get<PongMatch>(matchView.front());
	match.scoreLeft = 0;
	match.scoreRight = 0;
	match.phase = PongMatchPhase::Serving;
	match.phaseTime = 0.0f;

	for (auto [entity, scoreDisplay, transform] : registry.view<ScoreDisplay, Transform>().each()) {
		transform.setScale(scoreDisplay.baseScale);
	}

	_updateScoreDisplays(registry, match);
	_resetBall(registry);
	_resetPlayers(registry, true);
}

bool PongMatchSystem::isGameOver(entt::registry& registry) {
	auto matchView = registry.view<PongMatch>();
	return !matchView.empty() && matchView.get<PongMatch>(matchView.front()).phase == PongMatchPhase::GameOver;
}

void PongMatchSystem::_onPointScored(entt::registry& registry, PongMatch& match, PlayerSide scorer) {
	auto& score = scorer == PlayerSide::Left ? match.scoreLeft : match.scoreRight;
	++score;
	// The next ball is served towards the player who lost the point
	match.serveTowards = scorer == PlayerSide::Left ? PlayerSide::Right : PlayerSide::Left;
	match.phaseTime = 0.0f;

	_updateScoreDisplays(registry, match);
	_resetBall(registry);

	if (score >= match.winningScore) {
		match.phase = PongMatchPhase::GameOver;
		_resetPlayers(registry, false);
		LOG_INFO(Log::Game,
				 "{} player wins the match {} - {}",
				 scorer == PlayerSide::Left ? "Left" : "Right",
				 match.scoreLeft,
				 match.scoreRight);
	} else {
		match.phase = PongMatchPhase::Serving;
		_resetPlayers(registry, true);
	}
}

void PongMatchSystem::_resetBall(entt::registry& registry) {
	auto view = registry.view<Transform, TranslateItem, RotateItem, BallMovement>();
	for (auto [entity, transform, translateItem, rotateItem, ballMovement] : view.each()) {
		transform.setPosition({0.0f, 0.0f, transform.getPosition().z});
		translateItem.axis = Vector3::zero;
		translateItem.speed = 0.0f;
		rotateItem.speed = 0.0f;
	}
}

void PongMatchSystem::_serveBall(entt::registry& registry, const PongMatch& match) {
	auto view = registry.view<TranslateItem, RotateItem, BallMovement>();
	for (auto [entity, translateItem, rotateItem, ballMovement] : view.each()) {
		auto horizontalDirection = match.serveTowards == PlayerSide::Left ? -1.0f : 1.0f;
		auto angle = glm::radians(Random::range(-30.0f, 30.0f));
		translateItem.axis = {horizontalDirection * std::cos(angle), std::sin(angle), 0.0f};
		translateItem.speed = match.ballStartSpeed;
		rotateItem.speed = match.ballStartRotationSpeed;
	}
}

void PongMatchSystem::_resetPlayers(entt::registry& registry, bool active) {
	for (auto [entity, playerMovement, transform] : registry.view<PlayerMovement, Transform>().each()) {
		auto position = transform.getPosition();
		position.y = 0.0f;
		transform.setPosition(position);
		playerMovement.active = active;
	}
}

void PongMatchSystem::_updateScoreDisplays(entt::registry& registry, const PongMatch& match) {
	for (auto [entity, scoreDisplay, sprite] : registry.view<ScoreDisplay, Sprite>().each()) {
		auto score = scoreDisplay.side == PlayerSide::Left ? match.scoreLeft : match.scoreRight;
		sprite.spriteIndex = std::min(score, sprite.spriteSheet->getSpriteCount() - 1);
	}
}

} // namespace tactics::component
