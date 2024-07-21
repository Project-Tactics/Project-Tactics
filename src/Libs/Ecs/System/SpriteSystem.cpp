#include "SpriteSystem.h"

#include "../Component/MeshComponent.h"
#include "../Component/SpriteComponent.h"

#include <Libs/Utility/Time/EngineTime.h>

namespace tactics::component {

void SpriteSystem::update(entt::registry& registry) {
	auto view = registry.view<Sprite, Mesh>();
	view.each([](auto& sprite, auto& mesh) {
		auto& material = mesh.materials[0];
		material->set("u_SpriteUV", sprite.spriteSheet->getUVCoordinates(sprite.spriteIndex));
		material->set("u_SpriteSize", sprite.spriteSheet->getUVSpriteSize());
		material->set("u_FlipUV", sprite.uvFlip);
		material->set("u_Texture", sprite.spriteSheet->texture);
	});
}

void SpriteAnimationSystem::update(entt::registry& registry) {
	auto view = registry.view<Sprite, SpriteAnimation>();
	view.each([](auto& sprite, auto& spriteAnimation) {
		if (!spriteAnimation.currentAnimation.isEmpty()) {
			auto& animation = sprite.spriteSheet->animations.at(spriteAnimation.currentAnimation);

			spriteAnimation.currentTime += EngineTime::fixedDeltaTime<float>();
			if (spriteAnimation.currentTime >= animation.duration) {
				spriteAnimation.currentTime = 0.0f;
			}
			auto frameIndex =
				static_cast<unsigned int>(spriteAnimation.currentTime / animation.duration * animation.frames.size());
			sprite.spriteIndex = animation.frames[frameIndex];
		}
	});
}

} // namespace tactics::component
