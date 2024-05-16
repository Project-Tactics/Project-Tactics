#include "Renderer.h"
#include "RenderSteps/RenderStep.h"

namespace tactics {
void Renderer::render() {
	for (auto& renderStep : _renderSteps) {
		renderStep->render();
	}
}

}
