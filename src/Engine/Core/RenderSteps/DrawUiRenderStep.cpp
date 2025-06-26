#include "DrawUiRenderStep.h"

#include "Libs/Ui/UiSystem.h"

namespace tactics::renderstep {

struct UiRenderCommand {};

DrawUi::DrawUi(jab::UiSystem& uiSystem)
	: _uiSystem(uiSystem) {
}

void DrawUi::execute(RenderStepInfo&) {
}

} // namespace tactics::renderstep
