#pragma once

#include <Libs/Overlay/Overlay.h>

namespace tactics {

class EngineCoreOverlay : public Overlay {
public:
	EngineCoreOverlay();
	void update() override;
	OverlayConfig getConfig() override;
};

} // namespace tactics
