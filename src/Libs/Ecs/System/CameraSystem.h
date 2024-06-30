#pragma once

#include "../Component/CameraComponent.h"
#include "../Component/FrustumComponent.h"
#include "../Component/TransformComponent.h"
#include "../Component/ViewportComponent.h"

#include <Libs/Ecs/EntityUtilities.h>

namespace tactics::component {

class CameraSystem {
public:
	static void updateCameraMatrices(const ecs_view<Frustum, Transform, Camera>& view);
	static void updateCameraAspectRatios(const ecs_view<Viewport, CurrentViewport>& view1,
										 const ecs_view<Frustum, CurrentCamera>& view2);
};

} // namespace tactics::component
