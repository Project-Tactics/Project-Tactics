#pragma once

#include "../Component/BillboardComponent.h"
#include "../Component/CameraComponent.h"
#include "../Component/TransformComponent.h"
#include "../EntityUtilities.h"

namespace tactics::component {

class BillboardSystem {
public:
	static void update(ecs_view<Transform, Camera, CurrentCamera>& currentCameraView,
					   ecs_view<Transform, Billboard>& view);
};

} // namespace tactics::component
