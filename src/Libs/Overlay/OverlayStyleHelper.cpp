#include "OverlayStyleHelper.h"

#include <Libs/Resource/IniFile/IniFile.h>
#include <Libs/FileSystem/FileSystem.h>

#include <imgui/imgui.h>

#include <sstream>
#include <array>

// TODO(Gerark) Move this to a utility file, especially if we have other use cases
std::array<float, 4> ParseStringToFloats(const std::string& str) {
	std::array<float, 4> result = {0.0f, 0.0f, 0.0f, 0.0f};
	std::stringstream ss(str);
	std::string temp;
	size_t index = 0;

	while (std::getline(ss, temp, ',') && index < result.size()) {
		result[index++] = std::stof(temp);
	}

	return result;
}

template<>
struct ini::Convert<ImVec2> {
	void decode(const std::string& value, ImVec2& result) {
		auto floats = ParseStringToFloats(value);
		result.x = floats[0];
		result.y = floats[1];
	}

	void encode(const ImVec2 value, std::string& result) {
		result = std::to_string(value.x) + "," + std::to_string(value.y);
	}
};

template<>
struct ini::Convert<ImVec4> {
	void decode(const std::string& value, ImVec4& result) {
		auto floats = ParseStringToFloats(value);
		result.x = floats[0];
		result.y = floats[1];
		result.z = floats[2];
		result.w = floats[3];
	}

	void encode(const ImVec4 value, std::string& result) {
		result = std::to_string(value.x) + "," + std::to_string(value.y) + "," + std::to_string(value.z) + "," + std::to_string(value.w);
	}
};

namespace tactics {

void OverlayStyleHelper::setupImGuiStyle(const resource::IniFile& iniFile, const FileSystem& fileSystem) {
	ImGuiStyle* style = &ImGui::GetStyle();

	// TODO(Gerark) Yes, we should have a serializer for all of this.
	std::string category = "imguiSpacing";
	auto& file = iniFile.fileHandle->getObject();
	style->WindowPadding = file.at(category).at("windowPadding").as<ImVec2>();
	style->WindowRounding = file.at(category).at("windowRounding").as<float>();
	style->FramePadding = file.at(category).at("framePadding").as<ImVec2>();
	style->FrameRounding = file.at(category).at("frameRounding").as<float>();
	style->ItemSpacing = file.at(category).at("itemSpacing").as<ImVec2>();
	style->ItemInnerSpacing = file.at(category).at("itemInnerSpacing").as<ImVec2>();
	style->IndentSpacing = file.at(category).at("indentSpacing").as<float>();
	style->ScrollbarSize = file.at(category).at("scrollbarSize").as<float>();
	style->ScrollbarRounding = file.at(category).at("scrollbarRounding").as<float>();
	style->GrabMinSize = file.at(category).at("grabMinSize").as<float>();
	style->GrabRounding = file.at(category).at("grabRounding").as<float>();

	category = "imguiColors";
	style->Colors[ImGuiCol_Text] = file.at(category).at("text").as<ImVec4>();
	style->Colors[ImGuiCol_TextDisabled] = file.at(category).at("textDisabled").as<ImVec4>();
	style->Colors[ImGuiCol_WindowBg] = file.at(category).at("windowBg").as<ImVec4>();
	style->Colors[ImGuiCol_ChildBg] = file.at(category).at("childBg").as<ImVec4>();
	style->Colors[ImGuiCol_PopupBg] = file.at(category).at("popupBg").as<ImVec4>();
	style->Colors[ImGuiCol_Border] = file.at(category).at("border").as<ImVec4>();
	style->Colors[ImGuiCol_BorderShadow] = file.at(category).at("borderShadow").as<ImVec4>();
	style->Colors[ImGuiCol_FrameBg] = file.at(category).at("frameBg").as<ImVec4>();
	style->Colors[ImGuiCol_FrameBgHovered] = file.at(category).at("frameBgHovered").as<ImVec4>();
	style->Colors[ImGuiCol_FrameBgActive] = file.at(category).at("frameBgActive").as<ImVec4>();
	style->Colors[ImGuiCol_TitleBg] = file.at(category).at("titleBg").as<ImVec4>();
	style->Colors[ImGuiCol_TitleBgCollapsed] = file.at(category).at("titleBgCollapsed").as<ImVec4>();
	style->Colors[ImGuiCol_TitleBgActive] = file.at(category).at("titleBgActive").as<ImVec4>();
	style->Colors[ImGuiCol_MenuBarBg] = file.at(category).at("menuBarBg").as<ImVec4>();
	style->Colors[ImGuiCol_ScrollbarBg] = file.at(category).at("scrollbarBg").as<ImVec4>();
	style->Colors[ImGuiCol_ScrollbarGrab] = file.at(category).at("scrollbarGrab").as<ImVec4>();
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = file.at(category).at("scrollbarGrabHovered").as<ImVec4>();
	style->Colors[ImGuiCol_ScrollbarGrabActive] = file.at(category).at("scrollbarGrabActive").as<ImVec4>();
	style->Colors[ImGuiCol_CheckMark] = file.at(category).at("checkMark").as<ImVec4>();
	style->Colors[ImGuiCol_SliderGrab] = file.at(category).at("sliderGrab").as<ImVec4>();
	style->Colors[ImGuiCol_SliderGrabActive] = file.at(category).at("sliderGrabActive").as<ImVec4>();
	style->Colors[ImGuiCol_Button] = file.at(category).at("button").as<ImVec4>();
	style->Colors[ImGuiCol_ButtonHovered] = file.at(category).at("buttonHovered").as<ImVec4>();
	style->Colors[ImGuiCol_ButtonActive] = file.at(category).at("buttonActive").as<ImVec4>();
	style->Colors[ImGuiCol_Header] = file.at(category).at("header").as<ImVec4>();
	style->Colors[ImGuiCol_HeaderHovered] = file.at(category).at("headerHovered").as<ImVec4>();
	style->Colors[ImGuiCol_HeaderActive] = file.at(category).at("headerActive").as<ImVec4>();
	style->Colors[ImGuiCol_ResizeGrip] = file.at(category).at("resizeGrip").as<ImVec4>();
	style->Colors[ImGuiCol_ResizeGripHovered] = file.at(category).at("resizeGripHovered").as<ImVec4>();
	style->Colors[ImGuiCol_ResizeGripActive] = file.at(category).at("resizeGripActive").as<ImVec4>();
	style->Colors[ImGuiCol_PlotLines] = file.at(category).at("plotLines").as<ImVec4>();
	style->Colors[ImGuiCol_PlotLinesHovered] = file.at(category).at("plotLinesHovered").as<ImVec4>();
	style->Colors[ImGuiCol_PlotHistogram] = file.at(category).at("plotHistogram").as<ImVec4>();
	style->Colors[ImGuiCol_PlotHistogramHovered] = file.at(category).at("plotHistogramHovered").as<ImVec4>();
	style->Colors[ImGuiCol_TextSelectedBg] = file.at(category).at("textSelectedBg").as<ImVec4>();

	category = "imguiFonts";
	// TODO(Gerark) FileSystem: We should provide a FileHandle to the font file instead of relying on this custom code.
	std::filesystem::path fontPath = fileSystem.makeAbsolutePath(file.at(category).at("fontPath").as<std::string>());
	if (std::filesystem::exists(fontPath)) {
		auto fontPathStr = fontPath.string();
		auto& io = ImGui::GetIO();

		std::string fontSizes = file.at(category).at("fontSize").as<std::string>();
		std::string defaultSize = file.at(category).at("defaultFontSize").as<std::string>();
		std::stringstream ss(fontSizes);

		std::string str;
		while (std::getline(ss, str, ',')) {
			auto font = io.Fonts->AddFontFromFileTTF(fontPathStr.c_str(), static_cast<float>(std::atof(str.c_str())));
			if (str == defaultSize) {
				io.FontDefault = font;
			}
		}
	}
}
}
