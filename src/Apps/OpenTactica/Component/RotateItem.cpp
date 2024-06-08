#include "RotateItem.h"

#include <Libs/Ecs/EntityUtilities.h>
#include <Libs/Ecs/Entity.h>

namespace tactics::component {

void RotateItem::defineReflection() {
	entt::meta<RotateItem>()
		.type(hash("rotateItem"))
		.func<&Entity::explicitAddComponent<RotateItem>>(hash("emplace"))
		.data<&RotateItem::speed>(hash("speed"))
		.data<&RotateItem::axis>(hash("axis"));
}

void RotateItemSystem::update(const ecs_view<Transform, RotateItem>& view) {
	view.each([] (auto& transform, auto& rotateItem) {
		transform.rotate(rotateItem.speed, rotateItem.axis);
	});
}

}
