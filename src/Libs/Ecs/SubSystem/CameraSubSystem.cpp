#include "CameraSubSystem.h"

#include "../Component/CameraComponent.h"
#include "../Component/FrustumComponent.h"
#include "../Component/TransformComponent.h"
#include "../Component/ViewportComponent.h"

namespace tactics {

void CameraSubSystem::update() {
	using namespace component;

	// Update all camera matrices
	_reg().view<Frustum, Transform, Camera>().each([] (auto& frustum, auto& transform, auto& camera) {
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

	// Update the main camera aspect ratio
	_reg().view<Viewport, CurrentViewport>().each([this] (Viewport& viewport) {
		_reg().view<Frustum, CurrentCamera>().each([&viewport] (Frustum& frustum) {
			frustum.aspectRatio = static_cast<float>(viewport.size.x) / viewport.size.y;
		});
	});
}

}
