#include "TransformSystem.h"

namespace tactics::component {

void TransformSystem::updateTransformMatrices(ecs_view<Transform>& view) {
	view.each([] (Transform& transform) {
		transform.computeMatrix();
	});
}

}
