#pragma once

#include <vector>
#include <memory>

namespace tactics {

class RenderStep;
class Camera;

/**
 * @brief The RenderQueue class manages a sequence of rendering steps, ensuring each registered item is rendered
 * in a specific order, while providing contextual rendering information.
 */
class RenderQueue {
public:
	void execute(Camera& camera);

	template<typename TRenderStep, typename ...TArgs>
	void addStep(TArgs&&... args) {
		auto renderStep = std::make_unique<TRenderStep>(std::forward<TArgs&&>(args)...);
		_renderSteps.push_back(std::move(renderStep));
	}

private:
	std::vector<std::unique_ptr<RenderStep>> _renderSteps;
};

}
