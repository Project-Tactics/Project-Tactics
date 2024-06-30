#pragma once

#include "BattleCamera.h"

#include <Libs/Ecs/Component/SpriteComponent.h>
#include <Libs/Ecs/EntityUtilities.h>
#include <Libs/Utility/Reflection.h>

namespace tactics::component {

enum class Facing {
	North,
	South,
	East,
	West
};

NLOHMANN_JSON_SERIALIZE_ENUM(
	Facing,
	{{Facing::North, "north"}, {Facing::South, "south"}, {Facing::East, "east"}, {Facing::West, "west"}});

struct CharacterFacing {
	Facing facing = Facing::South;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(CharacterFacing, facing);
	static void defineReflection();
};

class CharacterFacingSystem {
public:
	static void update(BattleCamera& battleCamera,
					   ecs_view<CharacterFacing, SpriteAnimation, Sprite>& characterFacingView);

private:
	static void
	_updateAnimation(SpriteAnimation& animation, Sprite& sprite, const HashId& animationName, const glm::vec2& uvFlip);
};

} // namespace tactics::component
