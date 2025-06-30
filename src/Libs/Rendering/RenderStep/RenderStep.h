#pragma once

#include "../Viewport.h"

#include <Libs/Ecs/Component/CameraComponent.h>
#include <Libs/Ecs/Component/TransformComponent.h>

namespace tactics {

struct RenderStepInfo {
	const Viewport& viewport;
	const glm::vec2& windowSize;
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 viewProjection;
	glm::vec3 cameraPosition;
	glm::vec3 cameraForward;
};

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
