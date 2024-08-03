#include "CameraSystem.h"

#include "../Component/CameraComponent.h"
#include "../Component/FrustumComponent.h"
#include "../Component/TransformComponent.h"

#include <Libs/Rendering/RenderSystem.h>
#include <Libs/Rendering/Viewport.h>

namespace tactics::component {

void CameraSystem::updateCameraMatrices(entt::registry& registry) {
	auto view = registry.view<Frustum, Transform, Camera>();
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

void CameraSystem::updateCameraAspectRatios(RenderSystem& renderSystem, entt::registry& registry) {
	auto& viewport = renderSystem.getViewport();
	auto currentCameraView = registry.view<Frustum, CurrentCamera>();
	if (currentCameraView.size_hint() == 0) {
		return;
	}

	auto&& [entityCamera, frustum] = *currentCameraView.each().begin();
	auto windowSize = renderSystem.getWindowSize();
	frustum.aspectRatio = static_cast<float>(viewport.size.x * windowSize.x) / (viewport.size.y * windowSize.y);
}

} // namespace tactics::component
