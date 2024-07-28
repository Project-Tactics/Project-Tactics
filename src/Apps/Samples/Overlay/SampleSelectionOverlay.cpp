#include "../Overlay/SampleSelectionOverlay.h"

#include <Engine/Overlay/CustomOverlayColors.h>

#include <Libs/Rendering/RenderSystem.h>
#include <Libs/Utility/ImGuiUtilities.h>

namespace tactics {

SampleSelectionOverlay::SampleSelectionOverlay(RenderSystem& renderSystem,
											   const SampleFlows& sampleFlows,
											   std::function<void(const std::string&)> onSampleSelected)
	: _renderSystem(renderSystem)
	, _sampleFlows(sampleFlows)
	, _onSampleSelected(onSampleSelected) {}

void SampleSelectionOverlay::update() {
	CenteredTextColored(CustomOverlayColors::Colors::TitleTextColor, "Select Sample");

	for (const auto& [key, flow] : _sampleFlows) {
		if (CenteredButton(key.c_str(), {200, 25})) {
			_onSampleSelected(key);
		}
	}
}

OverlayConfig SampleSelectionOverlay::getConfig() {
	OverlayConfig config;
	auto windowSize = _renderSystem.getWindowSize();
	auto borders = ImVec2{windowSize.x * 0.35f, windowSize.y * 0.1f};
	config.position = {borders.x, borders.y};
	config.size = {windowSize.x - borders.x * 2, windowSize.y - borders.y * 2};
	config.windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar;
	config.windowSaveFlags = ImGuiCond_Always;
	return config;
}

} // namespace tactics
