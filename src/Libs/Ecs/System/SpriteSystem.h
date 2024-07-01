#pragma once

#include <entt/entt.hpp>

namespace tactics::component {

class SpriteSystem {
public:
	static void update(entt::registry& registry);
};

class SpriteAnimationSystem {
public:
	static void update(entt::registry& registry);
};

} // namespace tactics::component
