#pragma once

#include <Libs/Overlay/Overlay.h>

namespace tactics {
class EntityComponentSystem;
class RenderSystem;

namespace resource {
class ResourceSystem;
}

class PrototypeOverlay : public Overlay {
public:
	PrototypeOverlay(RenderSystem& renderSystem, EntityComponentSystem& ecs, resource::ResourceSystem& resourceSystem);
	void update() override;
	OverlayConfig getConfig() override;

private:
	RenderSystem& _renderSystem;
	resource::ResourceSystem& _resourceSystem;
	EntityComponentSystem& _ecs;
};

} // namespace tactics
