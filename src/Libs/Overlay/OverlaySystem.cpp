#include "OverlaySystem.h"

#include "OverlayStyleHelper.h"
#include "Overlay.h"

#include <Libs/Utility/Exception.h>
#include <Libs/Resource/IniFile/IniFile.h>

#include <imgui/imgui.h>

namespace tactics {

OverlaySystem::OverlaySystem(std::shared_ptr<resource::IniFile> devUserSettings, resource::IniFile& imGuiSettings, const FileSystem& fileSystem): _devUserSettings(devUserSettings) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	OverlayStyleHelper::setupImGuiStyle(imGuiSettings, fileSystem);
}

OverlaySystem::~OverlaySystem() {
	ImGui::DestroyContext();
}

void OverlaySystem::_addOverlay(std::string_view name, std::unique_ptr<Overlay> overlay, OverlayType type, bool enabled) {
	if (_overlays.contains(name)) {
		throw TACTICS_EXCEPTION("Can't add overlay \"{}\". Another overlay with the same name already exists.", name);
	}

	enabled = _getOrCreateOverlayStoredEnableValue(name, enabled);
	auto config = overlay->getConfig();
	_overlays.insert({name, OverlayItem{std::move(overlay), config, type, enabled}});
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
				ImGui::SetNextWindowPos(overlayItem.config.position, ImGuiCond_FirstUseEver);
				ImGui::SetNextWindowSize(overlayItem.config.size, ImGuiCond_FirstUseEver);
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
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
				auto openMainMenu = ImGui::BeginMainMenuBar();
				ImGui::PopStyleColor(1);
				if (openMainMenu) {
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
		throw TACTICS_EXCEPTION("Can't {} overlay \"{}\". Overlay not found.", name, enabled ? "enable" : "disable");
	}

	auto& overlay = _overlays[name];
	if (enabled != overlay.enabled) {
		_setOverlayStoredEnableValue(name, enabled);
	}

	overlay.enabled = enabled;
}

bool OverlaySystem::_getOrCreateOverlayStoredEnableValue(std::string_view name, bool defaultValue) {
	return _devUserSettings->getOrCreate("overlay", name, defaultValue);
}

void OverlaySystem::_setOverlayStoredEnableValue(std::string_view name, bool enabled) {
	_devUserSettings->set("overlay", name.data(), enabled);
	_devUserSettings->fileHandle->save();
}

}
