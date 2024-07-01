#include "TransformSystem.h"

#include "../Component/TransformComponent.h"

namespace tactics::component {

void TransformSystem::updateTransformMatrices(entt::registry& registry) {
	auto view = registry.view<Transform>();
	view.each([](Transform& transform) { transform.computeMatrix(); });
}

} // namespace tactics::component
