#pragma once

#include "../Component/MeshComponent.h"
#include "../Component/SpriteComponent.h"

namespace tactics::component {

class SpriteSystem {
public:
	static void update(const ecs_view<Sprite, Mesh>& view);
};

class SpriteAnimationSystem {
public:
	static void update(const ecs_view<Sprite, SpriteAnimation>& view);
};

} // namespace tactics::component
