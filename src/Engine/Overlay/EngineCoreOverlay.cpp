#include "EngineCoreOverlay.h"

#include "CustomOverlayColors.h"

#include <Libs/Utility/Time/EngineTime.h>

#include <imgui/imgui.h>

namespace tactics {

EngineCoreOverlay::EngineCoreOverlay() {}

void EngineCoreOverlay::update() {
	ImGui::TextColored(CustomOverlayColors::Colors::TitleTextColor, "%s", "Time");
	ImGui::Text("Fixed Delta Time: %f", EngineTime::fixedDeltaTime<float>());
}

OverlayConfig EngineCoreOverlay::getConfig() {
	OverlayConfig config;
	config.position = {5, 30};
	config.size = {300, 0};
	config.isMenuBarButton = true;
	return config;
}

} // namespace tactics
