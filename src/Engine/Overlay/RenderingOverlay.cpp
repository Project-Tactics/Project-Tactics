#include "RenderingOverlay.h"

#include "CustomOverlayColors.h"

#include <Libs/Ecs/Component/CameraComponent.h>
#include <Libs/Ecs/Component/FrustumComponent.h>
#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Ecs/EntityComponentSystem.h>
#include <Libs/Rendering/RenderSystem.h>
#include <Libs/Utility/ImGuiUtilities.h>

#include <string>

namespace tactics {

RenderingOverlay::RenderingOverlay(RenderSystem& renderSystem, EntityComponentSystem& ecs)
	: _renderSystem(renderSystem)
	, _ecs(ecs) {
	_titleColor = CustomOverlayColors::getColors().TitleTextColor;
	_xComponentColor = CustomOverlayColors::getColors().XComponentColor;
	_yComponentColor = CustomOverlayColors::getColors().YComponentColor;
	_zComponentColor = CustomOverlayColors::getColors().ZComponentColor;
}

OverlayConfig RenderingOverlay::getConfig() {
	OverlayConfig config;
	config.position = {5, 30};
	config.size = {300, 0};
	config.isMenuBarButton = true;
	return config;
}

void RenderingOverlay::update() {
	_drawRenderStats();
	ImGui::Separator();
	_drawCameraStats();
	ImGui::Separator();
	_drawViewportStats();
}

void RenderingOverlay::_drawRenderStats() {
	ImGui::TextColored(_titleColor, "%s", "STATS");
	ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
}

void RenderingOverlay::_drawViewportStats() {
	using namespace component;
	auto& viewport = _renderSystem.getViewport();
	ImGui::TextColored(_titleColor, "%s", "VIEWPORT");
	glm::vec2 position = viewport.position;
	glm::vec2 size = viewport.size;
	auto clearColor = viewport.clearColor;
	bool dirty = false;
	dirty |= vector2("TopLeft", position, 0, 1, _xComponentColor, _yComponentColor);
	dirty |= vector2("Size", size, 0, 1, _xComponentColor, _yComponentColor);
	if (dirty) {
		_renderSystem.setViewport(position, size, clearColor);
	}

	ImGui::AlignTextToFramePadding();
	ImGui::Text("%s", "Clear Color");
	ImGui::SameLine();
	ImVec4 color = ImVec4(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	if (ImGui::ColorEdit3("##clear-color", (float*)&color)) {
		_renderSystem.setViewport(position, size, clearColor);
	}
}

void RenderingOverlay::_drawCameraStats() {
	using namespace component;
	auto view = _ecs.sceneRegistry().view<Transform, Frustum, Camera>();
	view.each([&](Transform& transform, Frustum& frustum, Camera&) {
		ImGui::TextColored(_titleColor, "%s", "CAMERA");
		auto position = transform.getPosition();
		if (vector3("Position", position, 0, 0.1f, _xComponentColor, _yComponentColor, _zComponentColor)) {
			transform.setPosition(position);
		}

		glm::vec2 angles = glm::eulerAngles(transform.getRotation());
		if (vector2("Rotation", angles, 0, 0.01f, _xComponentColor, _yComponentColor)) {
			transform.setRotation(glm::quat(glm::vec3(angles.x, angles.y, 0.f)));
		}
		ImGui::Text("Fov: %f", frustum.fov);
		ImGui::Text("Near: %f", frustum.near);
		ImGui::Text("Far: %f", frustum.far);
		ImGui::Text("Aspect Ratio: %f", frustum.aspectRatio);
	});
}

} // namespace tactics
