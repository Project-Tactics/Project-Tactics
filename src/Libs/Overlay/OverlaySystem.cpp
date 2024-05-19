#include "OverlaySystem.h"
#include "Overlay.h"

#include <Libs/Utilities/Exception.h>
#include <Libs/Resource/IniFile/IniFile.h>

#include <imgui/imgui.h>

namespace tactics {

OverlaySystem::OverlaySystem(IniFile* iniFile): _iniFile(iniFile) {
}

OverlaySystem::~OverlaySystem() {}

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

}
