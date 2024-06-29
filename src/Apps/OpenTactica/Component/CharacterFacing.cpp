#include "CharacterFacing.h"

namespace tactics::component {

void CharacterFacing::defineReflection() {
	entt::meta<Facing>()
		.data<Facing::North>(hash("north"))
		.data<Facing::South>(hash("south"))
		.data<Facing::East>(hash("east"))
		.data<Facing::West>(hash("west"));

	componentReflection<CharacterFacing>("characterFacing")
		.data<&CharacterFacing::facing>(hash("facing"));
}

const std::array<std::array<Facing, 4>, 4> facingTransforms = {{
	{Facing::South, Facing::North, Facing::West, Facing::East},
	{Facing::East, Facing::West, Facing::South, Facing::North},
	{Facing::North, Facing::South, Facing::East, Facing::West},
	{Facing::West, Facing::East, Facing::North, Facing::South},
	}
};

void CharacterFacingSystem::update(
	BattleCamera& battleCamera,
	ecs_view<CharacterFacing, SpriteAnimation, Sprite>& characterFacingView) {

	auto index = static_cast<int>(glm::floor(battleCamera.getCurrentRotationDegree() / 90.f));

	for (auto [entity, facing, animation, sprite] : characterFacingView.each()) {
		auto transformedFacing = facingTransforms[index][static_cast<int>(facing.facing)];

		switch (transformedFacing) {
		case Facing::North:
			_updateAnimation(animation, sprite, "idleNorth", {-1, 1});
			break;
		case Facing::South:
			_updateAnimation(animation, sprite, "idleSouth", {1, 1});
			break;
		case Facing::East:
			_updateAnimation(animation, sprite, "idleSouth", {-1, 1});
			break;
		case Facing::West:
			_updateAnimation(animation, sprite, "idleNorth", {1, 1});
			break;
		}
	}
}

void CharacterFacingSystem::_updateAnimation(
	SpriteAnimation& animation,
	Sprite& sprite,
	const hash_string& animationName,
	const glm::vec2& uvFlip) {
	if (animation.currentAnimation != animationName || sprite.uvFlip != uvFlip) {
		animation.startAnimation(animationName);
		sprite.uvFlip = uvFlip;
	}
}

}