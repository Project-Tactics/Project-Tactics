#pragma once

#include "Overlay.h"

namespace tactics {

class ExampleOverlay : public Overlay {
public:
	virtual void update() override;

	static const auto TYPE = OverlayType::Custom;
};

} // namespace tactics
