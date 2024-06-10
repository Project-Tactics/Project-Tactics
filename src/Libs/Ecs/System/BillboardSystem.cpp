#include "BillboardSystem.h"

namespace tactics::component {

void BillboardSystem::update(ecs_view<Transform, Camera, CurrentCamera>& currentCameraView, ecs_view<Transform, Billboard>& view) {
	auto&& [entity, cameraTransform, camera] = *currentCameraView.each().begin();
	auto cameraRightWorldSpace = {camera.view[0][0], camera.view[1][0], camera.view[2][0]};
	auto cameraUpWorldSpace = {camera.view[0][1], camera.view[1][1], camera.view[2][1]};

	view.each([&cameraTransform] (Transform& transform) {
		(void)cameraTransform;
		(void)transform;
		auto diff = cameraTransform.getPosition() - transform.getPosition();
		diff.y = transform.getPosition().y;
		transform.lookAt(diff, Vector3::up);
	});
}

}
