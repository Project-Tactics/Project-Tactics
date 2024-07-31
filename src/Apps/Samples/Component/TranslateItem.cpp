#include "TranslateItem.h"

#include <Libs/Utility/Time/EngineTime.h>

namespace tactics::component {

void TranslateItemSystem::update(const ecs_view<Transform, TranslateItem>& view) {
	view.each([](auto& transform, auto& translateItem) {
		transform.translate(translateItem.axis * translateItem.speed * EngineTime::fixedDeltaTime<float>());
	});
}

} // namespace tactics::component
