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
	{Facing::West, Facing::East, Facing::North, Facing::South},
	{Facing::North, Facing::South, Facing::East, Facing::West},
	{Facing::East, Facing::West, Facing::South, Facing::North},
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
			sprite.spriteIndex = 3;
			sprite.uvFlip = {-1, 1};
			break;
		case Facing::South:
			sprite.spriteIndex = 1;
			sprite.uvFlip = {1, 1};
			break;
		case Facing::East:
			sprite.spriteIndex = 3;
			sprite.uvFlip = {1, 1};
			break;
		case Facing::West:
			sprite.spriteIndex = 1;
			sprite.uvFlip = {-1, 1};
			break;
		}
	}
}

}
