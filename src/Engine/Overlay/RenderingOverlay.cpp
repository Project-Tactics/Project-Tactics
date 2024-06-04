#include "RenderingOverlay.h"

#include "CustomOverlayColors.h"

#include <Libs/Rendering/RenderSystem.h>

#include <Libs/Ecs/EntityComponentSystem.h>
#include <Libs/Ecs/Component/CameraComponent.h>
#include <Libs/Ecs/Component/FrustumComponent.h>
#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Ecs/Component/ViewportComponent.h>

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
	auto view = _ecs.view<Viewport>();
	view.each([&] (Viewport& viewport) {
		ImGui::TextColored(_titleColor, "%s", "VIEWPORT");
		glm::vec2 topLeft = viewport.topLeft;
		glm::vec2 size = viewport.size;
		bool dirty = false;
		dirty |= _vector2("TopLeft", topLeft);
		dirty |= _vector2("Size", size);
		if (dirty) {
			viewport.topLeft = topLeft;
			viewport.size = size;
		}

		ImGui::AlignTextToFramePadding();
		ImGui::Text("%s", "Clear Color");
		auto clearColor = viewport.clearColor;
		ImGui::SameLine();
		ImVec4 color = ImVec4(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		if (ImGui::ColorEdit3("##clear-color", (float*)&color)) {
			viewport.clearColor = glm::vec4(color.x, color.y, color.z, clearColor.w);
		}
	});
}

void RenderingOverlay::_drawCameraStats() {
	using namespace component;
	auto view = _ecs.view<Transform, Frustum, Camera>();
	view.each([&] (Transform& transform, Frustum& frustum, Camera&) {
		ImGui::TextColored(_titleColor, "%s", "CAMERA");
		auto position = transform.getPosition();
		if (_vector3("Position", position, 0, 0.1f)) {
			transform.setPosition(position);
		}

		glm::vec2 angles = glm::eulerAngles(transform.getRotation());
		if (_vector2("Rotation", angles, 0, 0.01f)) {
			transform.setRotation(glm::quat(glm::vec3(angles.x, angles.y, 0.f)));
		}
		ImGui::Text("Fov: %f", frustum.fov);
		ImGui::Text("Near: %f", frustum.near);
		ImGui::Text("Far: %f", frustum.far);
		ImGui::Text("Aspect Ratio: %f", frustum.aspectRatio);
	});
}

bool RenderingOverlay::_vector3(const char* label, glm::vec3& vec, float componentWidth, float componentSpeed) {
	ImGui::AlignTextToFramePadding();
	ImGui::Text("%s", label);
	if (componentWidth == 0) {
		componentWidth = (ImGui::GetContentRegionAvail().x - (ImGui::CalcTextSize(label).x + ImGui::GetStyle().ItemSpacing.x * 4)) / 3;
	}
	ImGui::BeginGroup();
	ImGui::SameLine();
	bool valueChanged = false;
	valueChanged |= _vecComponent(&vec.x, _xComponentColor, componentWidth, componentSpeed, ("##x" + std::string(label)).c_str());
	ImGui::SameLine();
	valueChanged |= _vecComponent(&vec.y, _yComponentColor, componentWidth, componentSpeed, ("##y" + std::string(label)).c_str());
	ImGui::SameLine();
	valueChanged |= _vecComponent(&vec.z, _zComponentColor, componentWidth, componentSpeed, ("##z" + std::string(label)).c_str());
	ImGui::EndGroup();
	return valueChanged;
}

bool RenderingOverlay::_vector2(const char* label, glm::vec2& vec, float componentWidth, float componentSpeed) {
	ImGui::AlignTextToFramePadding();
	ImGui::Text("%s", label);
	if (componentWidth == 0) {
		componentWidth = (ImGui::GetContentRegionAvail().x - (ImGui::CalcTextSize(label).x + ImGui::GetStyle().ItemSpacing.x * 3)) / 2;
	}
	ImGui::BeginGroup();
	ImGui::SameLine();
	bool valueChanged = false;
	valueChanged |= _vecComponent(&vec.x, _xComponentColor, componentWidth, componentSpeed, ("##x" + std::string(label)).c_str());
	ImGui::SameLine();
	valueChanged |= _vecComponent(&vec.y, _yComponentColor, componentWidth, componentSpeed, ("##y" + std::string(label)).c_str());
	ImGui::EndGroup();
	return valueChanged;
}

bool RenderingOverlay::_vecComponent(float* component, const ImVec4& color, float width, float speed, const char* id) {
	ImVec2 squareSize(2, 23);
	ImVec2 cursorPos = ImGui::GetCursorScreenPos();
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->AddRectFilled(cursorPos, ImVec2(cursorPos.x + squareSize.x, cursorPos.y + squareSize.y), ImGui::ColorConvertFloat4ToU32(color));
	auto dummySize = ImVec2(squareSize.x - 12, squareSize.y);
	ImGui::Dummy(dummySize);

	ImGui::SameLine();
	ImGui::SetNextItemWidth(width);
	return ImGui::DragFloat(id, component, speed);
}

}
