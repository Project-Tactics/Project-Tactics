#pragma once

namespace tactics {

// TODO(Gerark): The step info can be renamed into some sort of context object
// where the RenderSteps can set information like current camera or other rendering information
struct RenderStepInfo {};

/**
 * @brief Interface implemented to handle rendering operations such as drawing meshes, setting clear color, applying
 * post-processing effects, etc.
 */
class RenderStep {
public:
	virtual ~RenderStep() = default;

	/**
	 * @brief Executes the rendering operation.
	 * @param camera The camera to use for rendering.
	 */
	virtual void execute(RenderStepInfo& renderInfo) = 0;
};

} // namespace tactics
