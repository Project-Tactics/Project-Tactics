#include "BillboardSystem.h"

#include "../Component/BillboardComponent.h"
#include "../Component/CameraComponent.h"
#include "../Component/TransformComponent.h"

namespace tactics::component {

void BillboardSystem::update(entt::registry& registry) {
	auto view = registry.view<Transform, Billboard>();
	auto currentCameraView = registry.view<Transform, Camera, CurrentCamera>();
	auto&& [entity, cameraTransform, camera] = *currentCameraView.each().begin();
	view.each([&cameraTransform](Transform& transform) {
		auto diff = transform.getPosition() - cameraTransform.getPosition();
		diff.y = transform.getPosition().y;
		transform.lookAt(diff, Vector3::up);
	});
}

} // namespace tactics::component
