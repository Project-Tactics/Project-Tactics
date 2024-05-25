#pragma once

#include "RenderStep.h"

namespace tactics {
class OverlaySystem;
}

namespace tactics::renderstep {

class ImGuiRender: public RenderStep {
public:
	ImGuiRender(OverlaySystem& overlaySystem);
	void execute(RenderStepInfo& renderInfo) override;

private:
	OverlaySystem& _overlaySystem;
};

class ImGuiRender1: public RenderStep {
public:
	void execute(RenderStepInfo& renderInfo) override;
};

}
