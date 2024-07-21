#pragma once

#include <Libs/Overlay/Overlay.h>

namespace tactics {
class InputSystem;

class InputOverlay : public Overlay {
public:
	InputOverlay(InputSystem& inputSystem);
	void update() override;
	OverlayConfig getConfig() override;

private:
	InputSystem& _inputSystem;
};

} // namespace tactics
