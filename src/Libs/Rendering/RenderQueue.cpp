#include "RenderQueue.h"
#include "RenderStep/RenderStep.h"

namespace tactics {
void RenderQueue::execute(RenderStepInfo& renderInfo) {
	for (auto& renderStep : _renderSteps) {
		renderStep->execute(renderInfo);
	}
}

}
