#include "CameraSystem.h"

namespace tactics::component {

void CameraSystem::updateCameraMatrices(const ecs_view<Frustum, Transform, Camera>& view) {
	view.each([] (auto& frustum, auto& transform, auto& camera) {
		auto target = transform.getPosition() + (transform.getRotation() * Vector3::forward);
		camera.view = glm::lookAt(transform.getPosition(), target, Vector3::up);
		switch (camera.projectionType) {
		case ProjectionType::Perspective: {
			camera.projection = glm::perspective(glm::radians(frustum.fov), frustum.aspectRatio, frustum.near, frustum.far);
			break;
		}
		case ProjectionType::Orthographic: {
			camera.projection = glm::ortho(
				-frustum.orthoSize * frustum.aspectRatio,
				frustum.orthoSize * frustum.aspectRatio,
				-frustum.orthoSize, frustum.orthoSize,
				frustum.near, frustum.far);
			break;
		}
		}
	});
}

void CameraSystem::updateCameraAspectRatios(const ecs_view<Viewport, CurrentViewport>& view1, const ecs_view<Frustum, CurrentCamera>& view2) {
	// TODO(Gerark) In theory we should go for a simple solution instead of iterating in a nested loop like that. This is working just cause
	// we have only one viewport and one camera in the scene. We should just assign a camera to each active viewport.
	view1.each([view2] (Viewport& viewport) {
		view2.each([&viewport] (Frustum& frustum) {
			frustum.aspectRatio = static_cast<float>(viewport.size.x) / viewport.size.y;
		});
	});
}

}
