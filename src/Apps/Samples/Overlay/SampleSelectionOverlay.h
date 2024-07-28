#pragma once

#include "../SamplesUtils.h"

#include <Libs/Overlay/Overlay.h>

namespace tactics {
class RenderSystem;

class SampleSelectionOverlay : public Overlay {
public:
	SampleSelectionOverlay(RenderSystem& renderSystem,
						   const SampleFlows& sampleFlows,
						   std::function<void(const std::string&)> onSampleSelected);
	void update() override;
	OverlayConfig getConfig() override;

private:
	RenderSystem& _renderSystem;
	const SampleFlows& _sampleFlows;
	std::function<void(const std::string&)> _onSampleSelected;
};

} // namespace tactics
