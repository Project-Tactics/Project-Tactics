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
	ImGuiCond_ saveFlags = ImGuiCond_Always;
	auto windowSize = _renderSystem.getWindowSize();
	auto borders = ImVec2{windowSize.x * 0.35f, windowSize.y * 0.1f};
	auto position = ImVec2{borders.x, borders.y};
	auto size = ImVec2{windowSize.x - borders.x * 2, windowSize.y - borders.y * 2};
	ImGui::SetNextWindowPos(position, saveFlags);
	ImGui::SetNextWindowSize(size, saveFlags);
	bool enabled = true;
	if (ImGui::Begin("SampleSelection",
					 &enabled,
					 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar)) {
		CenteredTextColored(CustomOverlayColors::Colors::TitleTextColor, "Select Sample");

		for (const auto& [key, flow] : _sampleFlows) {
			if (CenteredButton(key.c_str(), {200, 25})) {
				_onSampleSelected(key);
			}
		}
	}

	ImGui::End();
}

} // namespace tactics
