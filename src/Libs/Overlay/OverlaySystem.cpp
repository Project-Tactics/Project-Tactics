#include "OverlaySystem.h"
#include "Overlay.h"

#include <Libs/Utilities/Exception.h>

namespace tactics {

OverlaySystem::OverlaySystem() {
}

OverlaySystem::~OverlaySystem() {
}

void OverlaySystem::addOverlay(std::string_view name, std::unique_ptr<Overlay> overlay) {
	if (_overlays.contains(name)) {
		throw Exception("Can't add overlay \"{}\". Another overlay with the same name already exists.", name);
	}

	_overlays.insert({name, std::move(overlay)});
}

void OverlaySystem::removeOverlay(std::string_view name) {
	_overlays.erase(name);
}

void OverlaySystem::drawOverlays() {
	_update();
}

void OverlaySystem::_update() {
	if (!_isEnabled) {
		return;
	}

	//ImGui::ShowDemoWindow();

	for (auto&& [name, overlay] : _overlays) {
		overlay->update();
	}
}

bool OverlaySystem::isEnabled() const {
	return _isEnabled;
}

void OverlaySystem::setEnabled(bool enable) {
	_isEnabled = enable;
}

}
