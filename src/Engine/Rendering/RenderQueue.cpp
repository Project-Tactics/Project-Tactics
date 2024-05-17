#include "RenderQueue.h"
#include "RenderSteps/RenderStep.h"

namespace tactics {
void RenderQueue::render() {
	for (auto& renderStep : _renderSteps) {
		renderStep->render();
	}
}

}
