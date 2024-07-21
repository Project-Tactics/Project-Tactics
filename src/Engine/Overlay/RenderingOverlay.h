#pragma once

#include <Libs/Overlay/Overlay.h>

#include <glm/glm.hpp>
#include <imgui/imgui.h>

namespace tactics {

class EntityComponentSystem;
class RenderSystem;

class RenderingOverlay : public Overlay {
public:
	RenderingOverlay(RenderSystem& renderSystem, EntityComponentSystem& ecs);
	void update() override;
	OverlayConfig getConfig() override;

private:
	void _drawCameraStats();
	void _drawViewportStats();
	void _drawRenderStats();

	RenderSystem& _renderSystem;
	EntityComponentSystem& _ecs;
	ImVec4 _titleColor;
	ImVec4 _xComponentColor;
	ImVec4 _yComponentColor;
	ImVec4 _zComponentColor;
};

} // namespace tactics
