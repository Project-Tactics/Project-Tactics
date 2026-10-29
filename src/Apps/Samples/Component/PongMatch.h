#pragma once

#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Ecs/EntityUtilities.h>
#include <Libs/Utility/Reflection.h>

#include <glm/glm.hpp>

namespace tactics::component {

enum class PlayerSide {
	Left,
	Right
};

enum class PongMatchPhase {
	Serving,
	Playing,
	GameOver
};

/*
 * Rules and state of a pong match. The field is centered in the origin and fieldHalfSize defines its boundaries:
 * the ball bounces on the top/bottom ones and a player scores when the ball goes past the opponent's side.
 */
struct PongMatch {
	unsigned int winningScore = 9;
	glm::vec2 fieldHalfSize = {5.0f, 2.3f};
	float serveDelay = 1.0f;
	float ballStartSpeed = 2.5f;
	float ballStartRotationSpeed = 4.0f;

	PongMatchPhase phase = PongMatchPhase::Serving;
	unsigned int scoreLeft = 0;
	unsigned int scoreRight = 0;
	PlayerSide serveTowards = PlayerSide::Right;
	float phaseTime = 0.0f;

	COMPONENT(PongMatch, winningScore, fieldHalfSize, serveDelay, ballStartSpeed, ballStartRotationSpeed)
};

struct ScoreDisplay {
	PlayerSide side;
	glm::vec3 baseScale = Vector3::one;

	COMPONENT(ScoreDisplay, side, baseScale)
};

class PongMatchSystem {
public:
	static void update(entt::registry& registry);
	static void restartMatch(entt::registry& registry);
	[[nodiscard]] static bool isGameOver(entt::registry& registry);

private:
	static void _onPointScored(entt::registry& registry, PongMatch& match, PlayerSide scorer);
	static void _resetBall(entt::registry& registry);
	static void _serveBall(entt::registry& registry, const PongMatch& match);
	static void _resetPlayers(entt::registry& registry, bool active);
	static void _updateScoreDisplays(entt::registry& registry, const PongMatch& match);
};

} // namespace tactics::component
