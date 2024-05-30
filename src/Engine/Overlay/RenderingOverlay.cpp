#include "RenderingOverlay.h"

#include <Engine/Rendering/RenderSystem.h>
#include <Engine/Rendering/Camera.h>
#include <Engine/Rendering/Viewport.h>

#include <string>

namespace tactics {

const auto titleColor = ImVec4(0.059f, 0.600f, 0.851f, 1.f);
const auto xComponentColor = ImVec4(1.f, 0.f, 0.f, 1.f);
const auto yComponentColor = ImVec4(0, 1.f, 0.f, 1.f);
const auto zComponentColor = ImVec4(0.06f, 0.6f, 0.9f, 1.f);

RenderingOverlay::RenderingOverlay(RenderSystem& renderSystem)
	: _renderSystem(renderSystem) {
}

OverlayConfig RenderingOverlay::getConfig() {
	OverlayConfig config;
	config.position = {5, 30};
	config.size = {300, 0};
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
	ImGui::TextColored(titleColor, "%s", "STATS");
	ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
}

void RenderingOverlay::_drawViewportStats() {
	auto& viewport = _renderSystem.getViewport();
	ImGui::TextColored(titleColor, "%s", "VIEWPORT");
	glm::vec2 topLeft = viewport.getTopLeft();
	glm::vec2 size = viewport.getSize();
	bool dirty = false;
	dirty |= _vector2("TopLeft", topLeft);
	dirty |= _vector2("Size", size);
	if (dirty) {
		viewport.setDimensions(topLeft, size);
	}

	ImGui::AlignTextToFramePadding();
	ImGui::Text("%s", "Clear Color");
	auto clearColor = viewport.getClearColor();
	ImGui::SameLine();
	ImVec4 color = ImVec4(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	if (ImGui::ColorEdit3("##clear-color", (float*)&color)) {
		viewport.setClearColor(glm::vec4(color.x, color.y, color.z, clearColor.w));
	}
}

void RenderingOverlay::_drawCameraStats() {
	auto& camera = _renderSystem.getCamera();
	ImGui::TextColored(titleColor, "%s", "CAMERA");
	auto position = camera.getPosition();
	if (_vector3("Position", position, 0, 0.1f)) {
		camera.setPosition(position);
	}
	auto direction = camera.getDirection();
	if (_vector3("Direction", direction, 0, 0.01f)) {
		camera.setDirection(direction);
	}
	ImGui::Text("Fov: %f", camera.getFov());
	ImGui::Text("Near: %f", camera.getNearPlane());
	ImGui::Text("Far: %f", camera.getFarPlane());
	ImGui::Text("Aspect Ratio: %f", camera.getAspectRatio());
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
	valueChanged |= _vecComponent(&vec.x, xComponentColor, componentWidth, componentSpeed, ("##x" + std::string(label)).c_str());
	ImGui::SameLine();
	valueChanged |= _vecComponent(&vec.y, yComponentColor, componentWidth, componentSpeed, ("##y" + std::string(label)).c_str());
	ImGui::SameLine();
	valueChanged |= _vecComponent(&vec.z, zComponentColor, componentWidth, componentSpeed, ("##z" + std::string(label)).c_str());
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
	valueChanged |= _vecComponent(&vec.x, xComponentColor, componentWidth, componentSpeed, ("##x" + std::string(label)).c_str());
	ImGui::SameLine();
	valueChanged |= _vecComponent(&vec.y, yComponentColor, componentWidth, componentSpeed, ("##y" + std::string(label)).c_str());
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
