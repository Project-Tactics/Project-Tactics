#pragma once

#include "../Component/SpriteComponent.h"

namespace tactics::component {

class SpriteAnimationSystem {
public:
	static void update(const ecs_view<Sprite, SpriteAnimation>& view);
};

}
