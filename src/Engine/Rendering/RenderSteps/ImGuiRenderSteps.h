#pragma once

#include "RenderStep.h"

namespace tactics {
class OverlaySystem;
}

namespace tactics::renderstep {

class ImGuiBegin: public RenderStep {
public:
	void execute(Camera& camera) override;
};

class ImGuiEnd: public RenderStep {
public:
	void execute(Camera& camera) override;
};

class ImGuiRender: public RenderStep {
public:
	ImGuiRender(OverlaySystem& overlaySystem);
	void execute(Camera& camera) override;

private:
	OverlaySystem& _overlaySystem;
};

}
