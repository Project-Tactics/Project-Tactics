#pragma once

#include <Libs/Utility/Reflection.h>

namespace tactics::component {

struct CharAvatar {
	HashId textureId;
	COMPONENT(CharAvatar, textureId);
};

struct CharName {
	std::string name;

	COMPONENT(CharName, name);
};

struct CharStats {
	uint16_t hp{};
	uint16_t maxHp{};
	uint16_t mp{};
	uint16_t maxMp{};
	uint8_t level{};
	uint8_t xp{};
	uint8_t move{};
	uint8_t jump{};

	COMPONENT(CharStats, hp, maxHp, mp, maxMp, level, xp, move, jump);
};

struct ChargeTime {
	float speed{};
	float chargeTime{};

	COMPONENT(ChargeTime, chargeTime, speed);
};

struct CharBattleReady {
	COMPONENT_TAG(CharBattleReady);
};

class BattleSystem {
public:
	static void advanceTick(entt::registry& registry, float deltaTime);
};

} // namespace tactics::component
