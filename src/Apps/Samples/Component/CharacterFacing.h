#pragma once

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

struct CharacterFacing {
	Facing facing = Facing::South;

	REFLECT(CharacterFacing, facing);
};

class CharacterFacingSystem {
public:
	static void update(entt::registry& registry);

private:
	static void
	_updateAnimation(SpriteAnimation& animation, Sprite& sprite, const HashId& animationName, const glm::vec2& uvFlip);
};

} // namespace tactics::component
