#include "CameraSystem.h"

namespace tactics::component {

void CameraSystem::updateCameraMatrices(const ecs_view<Frustum, Transform, Camera>& view) {
	view.each([](auto& frustum, auto& transform, auto& camera) {
		auto target = transform.getPosition() + (transform.getRotation() * Vector3::forward);
		camera.view = glm::lookAt(transform.getPosition(), target, Vector3::up);
		switch (camera.projectionType) {
		case ProjectionType::Perspective: {
			camera.projection =
				glm::perspective(glm::radians(frustum.fov), frustum.aspectRatio, frustum.near, frustum.far);
			break;
		}
		case ProjectionType::Orthographic: {
			camera.projection = glm::ortho(-frustum.orthoSize * frustum.aspectRatio,
										   frustum.orthoSize * frustum.aspectRatio,
										   -frustum.orthoSize,
										   frustum.orthoSize,
										   frustum.near,
										   frustum.far);
			break;
		}
		}
	});
}

void CameraSystem::updateCameraAspectRatios(const ecs_view<Viewport, CurrentViewport>& currentViewportView,
											const ecs_view<Frustum, CurrentCamera>& currentCameraView) {
	// TODO(Gerark) We assume we always have one current viewport and current camera. We might change this in the future
	// and this has to change accordingly.
	auto&& [entityViewport, viewport] = *currentViewportView.each().begin();
	auto&& [entityCamera, frustum] = *currentCameraView.each().begin();
	(void)entityCamera;
	(void)entityViewport;
	frustum.aspectRatio = static_cast<float>(viewport.size.x) / viewport.size.y;
}

} // namespace tactics::component
