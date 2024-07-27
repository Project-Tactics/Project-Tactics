#include "RotateItem.h"

#include <Libs/Utility/Time/EngineTime.h>

namespace tactics::component {

void RotateItemSystem::update(entt::registry& registry) {
	auto view = registry.view<Transform, RotateItem>();
	view.each([](auto& transform, auto& rotateItem) {
		transform.rotate(rotateItem.speed * EngineTime::fixedDeltaTime<float>(), rotateItem.axis);
	});
}

} // namespace tactics::component
