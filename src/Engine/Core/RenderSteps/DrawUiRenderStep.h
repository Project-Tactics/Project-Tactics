#pragma once

#include <Libs/Rendering/RenderStep/RenderStep.h>

namespace jab {
class UiSystem;
}

namespace tactics::renderstep {

class DrawUi : public RenderStep {
public:
	DrawUi(jab::UiSystem& uiSystem);
	void execute(RenderStepInfo& renderInfo) override;

private:
	jab::UiSystem& _uiSystem;
};

} // namespace tactics::renderstep
