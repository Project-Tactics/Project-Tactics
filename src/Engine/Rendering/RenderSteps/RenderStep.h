#pragma once

namespace tactics {

class Camera;

/**
 * @brief Interface implemented to handle rendering operations such as drawing meshes, setting clear color, applying post-processing effects, etc.
 */
class RenderStep {
public:
	virtual ~RenderStep() = default;

	/**
	 * @brief Executes the rendering operation.
	 * @param camera The camera to use for rendering.
	 */
	virtual void execute(Camera& camera) = 0;
};

}
