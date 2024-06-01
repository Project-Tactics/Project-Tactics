#pragma once

#include <Libs/Rendering/RenderStep/RenderStep.h>

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

}
