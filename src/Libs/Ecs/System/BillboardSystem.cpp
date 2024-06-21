#include "BillboardSystem.h"

namespace tactics::component {

void BillboardSystem::update(ecs_view<Transform, Camera, CurrentCamera>& currentCameraView, ecs_view<Transform, Billboard>& view) {
	auto&& [entity, cameraTransform, camera] = *currentCameraView.each().begin();
	view.each([&cameraTransform] (Transform& transform) {
		auto diff = transform.getPosition() - cameraTransform.getPosition();
		diff.y = transform.getPosition().y;
		transform.lookAt(diff, Vector3::up);
	});
}

}
