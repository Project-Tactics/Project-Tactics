#include "ClearViewportRenderStep.h"

#include "../Viewport.h"

#include <glad/gl.h>

namespace tactics::renderstep {

void ClearViewport::execute(RenderStepInfo& renderInfo) {
	const auto& clearColor = renderInfo.viewport.getClearColor();
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

}
