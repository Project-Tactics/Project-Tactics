#pragma once

#include "Overlay.h"

namespace tactics {

class OverlaySystem;

class MainOverlay : public Overlay {
public:
	MainOverlay(OverlaySystem& overlaySystem);
	~MainOverlay();

	void update() override;

	static const auto TYPE = OverlayType::MenuBar;

private:
	OverlaySystem& _overlaySystem;
};

} // namespace tactics
