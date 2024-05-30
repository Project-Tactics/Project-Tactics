#include "MainOverlay.h"

#include "OverlaySystem.h"

#include <imgui/imgui.h>

namespace tactics {

MainOverlay::MainOverlay(OverlaySystem& overlaySystem)
	: _overlaySystem(overlaySystem) {
}

MainOverlay::~MainOverlay() {}

void MainOverlay::update() {
	if (ImGui::BeginMenu("Views")) {
		_overlaySystem.forEachOverlay([this] (const std::string& name, OverlaySystem::OverlayItem& overlayItem) {
			if (overlayItem.overlay.get() == this) {
				return;
			}
			bool enabled = overlayItem.enabled;
			if (ImGui::MenuItem(name.c_str(), nullptr, &enabled)) {
				_overlaySystem.enableOverlay(name, enabled);
			}
		});
		ImGui::EndMenu();
	}
}

}
