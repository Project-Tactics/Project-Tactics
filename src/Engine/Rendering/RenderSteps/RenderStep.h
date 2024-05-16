#pragma once

namespace tactics {

/**
 * @brief Interface implemented to handle rendering operations such as drawing meshes, setting clear color, applying post-processing effects, etc.
 */
class RenderStep {
public:
	virtual ~RenderStep() = default;

	/**
	 * @brief Renders the scene according to the implemented rendering step.
	 */
	virtual void render() = 0;
};

}
