#include "RotateItem.h"

namespace tactics::component {

void RotateItemSystem::update(const ecs_view<Transform, RotateItem>& view) {
	view.each([] (auto& transform, auto& rotateItem) {
		transform.rotate(rotateItem.speed, rotateItem.axis);
	});
}

}
