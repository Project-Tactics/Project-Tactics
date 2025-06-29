#pragma once

#include <Libs/Resource/Material/Material.h>
#include <Libs/Resource/Mesh/Mesh.h>
#include <Libs/Resource/SpriteSheet/SpriteSheet.h>
#include <Libs/Utility/Reflection.h>

#include <memory>

namespace tactics::component {

struct Sprite {
	std::shared_ptr<resource::SpriteSheet> spriteSheet;
	unsigned int spriteIndex;
	glm::vec2 uvFlip = Vector2::one;

	COMPONENT(Sprite, spriteSheet, spriteIndex, uvFlip)
};

struct SpriteAnimation {
	float currentTime = 0.0f;
	HashId currentAnimation = HashId::none;

	void startAnimation(const HashId& animation);

	COMPONENT(SpriteAnimation, currentTime, currentAnimation)
};

} // namespace tactics::component
