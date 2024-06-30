#include "RotateItem.h"

#include <Libs/Utility/Time/EngineTime.h>

namespace tactics::component {

void RotateItem::defineReflection() {
	componentReflection<RotateItem>("rotateItem")
		.data<&RotateItem::speed>("speed"_id)
		.data<&RotateItem::axis>("axis"_id);
}

void RotateItemSystem::update(const ecs_view<Transform, RotateItem>& view) {
	view.each([](auto& transform, auto& rotateItem) {
		transform.rotate(rotateItem.speed * EngineTime::fixedDeltaTime<float>(), rotateItem.axis);
	});
}

} // namespace tactics::component
