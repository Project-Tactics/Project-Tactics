#include "MainOverlay.h"

#include "OverlaySystem.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace tactics {

MainOverlay::MainOverlay(OverlaySystem& overlaySystem)
	: _overlaySystem(overlaySystem) {
}

MainOverlay::~MainOverlay() {}

void MainOverlay::update() {
	if (ImGui::BeginMenu("Other Views")) {
		_overlaySystem.forEachOverlay([this] (const std::string& name, OverlaySystem::OverlayItem& overlayItem) {
			if (overlayItem.overlay.get() == this) {
				return;
			}

			if (overlayItem.config.isMenuBarButton) {
				return;
			}

			bool enabled = overlayItem.enabled;
			if (ImGui::MenuItem(name.c_str(), nullptr, &enabled)) {
				_overlaySystem.enableOverlay(name, enabled);
			}
		});
		ImGui::EndMenu();
	}

	_overlaySystem.forEachOverlay([this] (const std::string& name, OverlaySystem::OverlayItem& overlayItem) {
		if (overlayItem.overlay.get() == this) {
			return;
		}

		if (overlayItem.config.isMenuBarButton) {
			ImGui::PushStyleColor(ImGuiCol_Button, overlayItem.enabled ? ImVec4(0.3f, 0.3f, 0.3f, 1.0f) : ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
			auto isClicked = ImGui::Button(name.c_str());
			if (isClicked) {
				_overlaySystem.enableOverlay(name, !overlayItem.enabled);
			}
			ImGui::PopStyleColor();
		}
	});
}

}
