#include "SpriteSystem.h"

namespace tactics::component {

void SpriteSystem::update(const ecs_view<Sprite, Mesh>& view) {
	view.each([] (auto& sprite, auto& mesh) {
		auto& material = mesh.materials[0];
		material->set("u_SpriteUV", sprite.spriteSheet->getUVCoordinates(sprite.spriteIndex));
		material->set("u_SpriteSize", sprite.spriteSheet->getUVSpriteSize());
		material->set("u_FlipUV", sprite.uvFlip);
		material->set("u_Texture", sprite.spriteSheet->texture);
	});
}

void SpriteAnimationSystem::update(const ecs_view<Sprite, SpriteAnimation>&) {
	/*
	view.each([] (auto& sprite, auto& spriteAnimation) {
		spriteAnimation.i++;
		if (spriteAnimation.i > 500) {
			spriteAnimation.i = 0;
		}

		sprite.spriteIndex = (int)spriteAnimation.i / 100;
	});
	*/
}

}
