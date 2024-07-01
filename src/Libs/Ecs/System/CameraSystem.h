#pragma once

#include <Libs/Ecs/EntityUtilities.h>

namespace tactics::component {

class CameraSystem {
public:
	static void updateCameraMatrices(entt::registry& registry);
	static void updateCameraAspectRatios(entt::registry& registry);
};

} // namespace tactics::component
