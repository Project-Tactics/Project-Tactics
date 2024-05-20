#pragma once

#include "RenderStep.h"

namespace tactics {
class OverlaySystem;
}

namespace tactics::renderstep {

class ImGuiBegin: public RenderStep {
public:
	void execute(RenderStepInfo& renderInfo) override;
};

class ImGuiEnd: public RenderStep {
public:
	void execute(RenderStepInfo& renderInfo) override;
};

class ImGuiRender: public RenderStep {
public:
	ImGuiRender(OverlaySystem& overlaySystem);
	void execute(RenderStepInfo& renderInfo) override;

private:
	OverlaySystem& _overlaySystem;
};

}
