#include "OverlaySystem.h"
#include "Overlay.h"

#include <Libs/Utility/Exception.h>
#include <Libs/Resource/IniFile/IniFile.h>
#include <Libs/Resource/ResourcePathHelper.h>

#include <imgui/imgui.h>

namespace tactics {

OverlaySystem::OverlaySystem(std::shared_ptr<resource::IniFile> iniFile, const resource::ResourcePathHelper& resourcePathHelper): _iniFile(iniFile) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	_setupImGuiStyle(resourcePathHelper);
}

OverlaySystem::~OverlaySystem() {
	ImGui::DestroyContext();
}

void OverlaySystem::_addOverlay(std::string_view name, std::unique_ptr<Overlay> overlay, OverlayType type, bool enabled) {
	if (_overlays.contains(name)) {
		throw Exception("Can't add overlay \"{}\". Another overlay with the same name already exists.", name);
	}

	enabled = _getOrCreateOverlayStoredEnableValue(name, enabled);
	_overlays.insert({name, OverlayItem{std::move(overlay), enabled, type}});
}

void OverlaySystem::removeOverlay(std::string_view name) {
	_overlays.erase(name);
}

void OverlaySystem::update() {
	if (!_isEnabled) {
		return;
	}

	ImGui::BeginGroup();
	for (auto&& [name, overlayItem] : _overlays) {
		if (overlayItem.enabled) {
			switch (overlayItem.type) {
			case OverlayType::Window: {
				bool enabled = overlayItem.enabled;
				if (ImGui::Begin(name.data(), &enabled)) {
					overlayItem.overlay->update();
				}
				ImGui::End();
				if (!enabled) {
					enableOverlay(name, false);
				}
				break;
			}
			case OverlayType::MenuBar: {
				if (ImGui::BeginMainMenuBar()) {
					overlayItem.overlay->update();
					ImGui::EndMainMenuBar();
				}
				break;
			}
			case OverlayType::Custom: {
				overlayItem.overlay->update();
				break;
			}
			}
		}
	}
	ImGui::EndGroup();
}

bool OverlaySystem::isEnabled() const {
	return _isEnabled;
}

void OverlaySystem::setEnabled(bool enable) {
	_isEnabled = enable;
}

void OverlaySystem::forEachOverlay(const std::function<void(const std::string&, OverlayItem&)>& callback) {
	for (auto&& [name, overlayItem] : _overlays) {
		callback(name, overlayItem);
	}
}

void OverlaySystem::enableOverlay(std::string_view name, bool enabled) {
	if (!_overlays.contains(name)) {
		throw Exception("Can't {} overlay \"{}\". Overlay not found.", name, enabled ? "enable" : "disable");
	}

	auto& overlay = _overlays[name];
	if (enabled != overlay.enabled) {
		_setOverlayStoredEnableValue(name, enabled);
	}

	overlay.enabled = enabled;
}

bool OverlaySystem::_getOrCreateOverlayStoredEnableValue(std::string_view name, bool defaultValue) {
	return _iniFile->getOrCreate("overlay", name, defaultValue);
}

void OverlaySystem::_setOverlayStoredEnableValue(std::string_view name, bool enabled) {
	_iniFile->set("overlay", name.data(), enabled);
	_iniFile->save();
}

void OverlaySystem::_setupImGuiStyle(const resource::ResourcePathHelper& resourcePathHelper) {
	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;

	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 0.80f);
	style->Colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.25f, 0.28f, 0.25f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.26f, 0.28f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);

	std::filesystem::path fontPath = resourcePathHelper.makeAbsolutePath("debug\\ruda-bold.ttf");
	if (std::filesystem::exists(fontPath)) {
		auto fontPathStr = fontPath.string();
		auto& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF(fontPathStr.c_str(), 10);
		io.Fonts->AddFontFromFileTTF(fontPathStr.c_str(), 12);
		auto defaultFont = io.Fonts->AddFontFromFileTTF(fontPathStr.c_str(), 14);
		io.FontDefault = defaultFont;
		io.Fonts->AddFontFromFileTTF(fontPathStr.c_str(), 18);
		io.Fonts->AddFontFromFileTTF(fontPathStr.c_str(), 24);
		io.Fonts->AddFontFromFileTTF(fontPathStr.c_str(), 32);
	}
}

}
