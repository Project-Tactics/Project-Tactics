#include "ImGuiUtilities.h"

#include <Libs/Resource/Texture/Texture.h>

namespace tactics {

void* toImGuiTexture(const resource::Texture& texture) {
	return (void*)(intptr_t)(texture.rendererId);
}

ImVec2 toImVec2(const glm::vec2& vec) {
	return ImVec2(vec.x, vec.y);
}

bool vector3(const char* label,
			 glm::vec3& vec,
			 float componentWidth,
			 float componentSpeed,
			 const ImVec4& xColor,
			 const ImVec4& yColor,
			 const ImVec4& zColor) {
	ImGui::AlignTextToFramePadding();
	ImGui::Text("%s", label);
	if (componentWidth == 0) {
		componentWidth =
			(ImGui::GetContentRegionAvail().x - (ImGui::CalcTextSize(label).x + ImGui::GetStyle().ItemSpacing.x * 4)) /
			3;
	}
	ImGui::BeginGroup();
	ImGui::SameLine();
	bool valueChanged = false;
	valueChanged |= vecComponent(&vec.x, xColor, componentWidth, componentSpeed, ("##x" + std::string(label)).c_str());
	ImGui::SameLine();
	valueChanged |= vecComponent(&vec.y, yColor, componentWidth, componentSpeed, ("##y" + std::string(label)).c_str());
	ImGui::SameLine();
	valueChanged |= vecComponent(&vec.z, zColor, componentWidth, componentSpeed, ("##z" + std::string(label)).c_str());
	ImGui::EndGroup();
	return valueChanged;
}

bool vector2(const char* label,
			 glm::vec2& vec,
			 float componentWidth,
			 float componentSpeed,
			 const ImVec4& xColor,
			 const ImVec4& yColor) {
	ImGui::AlignTextToFramePadding();
	ImGui::Text("%s", label);
	if (componentWidth == 0) {
		componentWidth =
			(ImGui::GetContentRegionAvail().x - (ImGui::CalcTextSize(label).x + ImGui::GetStyle().ItemSpacing.x * 3)) /
			2;
	}
	ImGui::BeginGroup();
	ImGui::SameLine();
	bool valueChanged = false;
	valueChanged |= vecComponent(&vec.x, xColor, componentWidth, componentSpeed, ("##x" + std::string(label)).c_str());
	ImGui::SameLine();
	valueChanged |= vecComponent(&vec.y, yColor, componentWidth, componentSpeed, ("##y" + std::string(label)).c_str());
	ImGui::EndGroup();
	return valueChanged;
}

bool vecComponent(float* component, const ImVec4& color, float width, float speed, const char* id) {
	ImVec2 squareSize(2, 23);
	ImVec2 cursorPos = ImGui::GetCursorScreenPos();
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->AddRectFilled(cursorPos,
							ImVec2(cursorPos.x + squareSize.x, cursorPos.y + squareSize.y),
							ImGui::ColorConvertFloat4ToU32(color));
	auto dummySize = ImVec2(squareSize.x - 12, squareSize.y);
	ImGui::Dummy(dummySize);

	ImGui::SameLine();
	ImGui::SetNextItemWidth(width);
	return ImGui::DragFloat(id, component, speed);
}

} // namespace tactics
