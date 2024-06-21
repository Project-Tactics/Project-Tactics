#include "SpriteSystem.h"

namespace tactics::component {

void SpriteAnimationSystem::update(const ecs_view<Sprite, SpriteAnimation>& view) {
	view.each([] (auto& sprite, auto& spriteAnimation) {
		spriteAnimation.i++;
		if (spriteAnimation.i > 500) {
			spriteAnimation.i = 0;
		}

		sprite.spriteIndex = (int)spriteAnimation.i / 100;
	});
}

}
