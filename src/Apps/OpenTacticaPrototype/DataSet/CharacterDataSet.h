#pragma once

#include <Libs/Resource/DataSet/DataSetReflection.h>

namespace tactics {

struct CharacterDataSet {
	std::string name;
	HashId avatarId;
	uint16_t hp{};
	uint16_t mp{};
	uint8_t level{};
	uint8_t xp{};
	uint8_t move{};
	uint8_t jump{};
	float speed{};

	DATASET(CharacterDataSet, name, avatarId, hp, mp, level, xp, move, jump, speed);
};

} // namespace tactics
