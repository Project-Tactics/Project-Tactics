#pragma once

#include "../Component/SpriteComponent.h"
#include "../Component/MeshComponent.h"

namespace tactics::component {

class SpriteSystem {
public:
	static void update(const ecs_view<Sprite, Mesh>& view);
};

class SpriteAnimationSystem {
public:
	static void update(const ecs_view<Sprite, SpriteAnimation>& view);
};

}
