#include "SpriteComponent.h"

namespace tactics::component {

void SpriteAnimation::startAnimation(const HashId& animation) {
	currentAnimation = animation;
	currentTime = 0.0f;
}

} // namespace tactics::component
