#include "RenderQueue.h"
#include "RenderSteps/RenderStep.h"

namespace tactics {
void RenderQueue::execute(Camera& camera) {
	for (auto& renderStep : _renderSteps) {
		renderStep->execute(camera);
	}
}

}
