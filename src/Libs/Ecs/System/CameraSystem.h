#pragma once

#include <Libs/Ecs/EntityUtilities.h>

namespace tactics {
class RenderSystem;
}

namespace tactics::component {

class CameraSystem {
public:
	static void updateCameraMatrices(entt::registry& registry);
	static void updateCameraAspectRatios(RenderSystem& renderSystem, entt::registry& registry);
};

} // namespace tactics::component
