#include "DebugOverlay.h"

#include <Engine/Rendering/RenderSystem.h>
#include <Engine/Rendering/Camera.h>

#include <string>

namespace tactics {

const auto titleColor = ImVec4(0.059f, 0.600f, 0.851f, 1.f);
const auto xComponentColor = ImVec4(1.f, 0.f, 0.f, 1.f);
const auto yComponentColor = ImVec4(0, 1.f, 0.f, 1.f);
const auto zComponentColor = ImVec4(0.06f, 0.6f, 0.9f, 1.f);

DebugOverlay::DebugOverlay(RenderSystem& renderSystem)
	: _renderSystem(renderSystem) {
}

void DebugOverlay::update() {
	if (ImGui::BeginTabBar("MyTabBar", ImGuiTabBarFlags_None)) {
		if (ImGui::BeginTabItem("Rendering")) {
			ImGui::TextColored(titleColor, "%s", "STATS");
			ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
			ImGui::EndTabItem();
			ImGui::Separator();
			auto& camera = _renderSystem.getCamera();
			ImGui::TextColored(titleColor, "%s", "CAMERA");
			auto& position = camera.getPosition();
			auto _pos = position;
			if (_vector3("Position", _pos)) {
				camera.setPosition(_pos);
			}
			ImGui::Text("Fov: %f", camera.getFov());
			ImGui::Text("Near: %f", camera.getNearPlane());
			ImGui::Text("Far: %f", camera.getFarPlane());
		}
		if (ImGui::BeginTabItem("Resources")) {
			ImGui::TextWrapped("Would be good to show all the resource pack + resources loaded. Probably it would be better to have 2 different windows as well.");
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
}

bool DebugOverlay::_vector3(const char* label, glm::vec3& vec, float componentWidth) {
	ImGui::AlignTextToFramePadding();
	ImGui::Text("%s", label);
	if (componentWidth == 0) {
		componentWidth = (ImGui::GetContentRegionAvail().x - (ImGui::CalcTextSize(label).x + ImGui::GetStyle().ItemSpacing.x * 4)) / 3;
	}
	ImGui::BeginGroup();
	ImGui::SameLine();
	bool valueChanged = false;
	valueChanged |= _vecComponent(&vec.x, xComponentColor, componentWidth, ("##x" + std::string(label)).c_str());
	ImGui::SameLine();
	valueChanged |= _vecComponent(&vec.y, yComponentColor, componentWidth, ("##y" + std::string(label)).c_str());
	ImGui::SameLine();
	valueChanged |= _vecComponent(&vec.z, zComponentColor, componentWidth, ("##z" + std::string(label)).c_str());
	ImGui::EndGroup();
	return valueChanged;
}

bool DebugOverlay::_vecComponent(float* component, const ImVec4& color, float width, const char* id) {
	ImVec2 squareSize(2, 23);
	ImVec2 cursorPos = ImGui::GetCursorScreenPos();
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->AddRectFilled(cursorPos, ImVec2(cursorPos.x + squareSize.x, cursorPos.y + squareSize.y), ImGui::ColorConvertFloat4ToU32(color));
	auto dummySize = ImVec2(squareSize.x - 12, squareSize.y);
	ImGui::Dummy(dummySize);

	ImGui::SameLine();
	ImGui::SetNextItemWidth(width);
	return ImGui::DragFloat(id, component, 0.1f);
}

}