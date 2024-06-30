#pragma once

#include <memory>
#include <vector>

namespace tactics {

struct RenderStepInfo;
class RenderStep;
class Camera;

/**
 * @brief The RenderQueue class manages a sequence of rendering steps meant to take care of the render pipeline.
 */
class RenderQueue {
public:
	void execute(RenderStepInfo& renderInfo);

	template<typename TRenderStep, typename... TArgs> void addStep(TArgs&&... args) {
		auto renderStep = std::make_unique<TRenderStep>(std::forward<TArgs&&>(args)...);
		_renderSteps.push_back(std::move(renderStep));
	}

private:
	std::vector<std::unique_ptr<RenderStep>> _renderSteps;
};

} // namespace tactics
