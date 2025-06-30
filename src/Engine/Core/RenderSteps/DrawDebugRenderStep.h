#pragma once

#include <Libs/Rendering/IndexBuffer.h>
#include <Libs/Rendering/RenderStep/RenderStep.h>
#include <Libs/Rendering/VertexAttributes.h>
#include <Libs/Rendering/VertexBuffer.h>
#include <Libs/Resource/Shader/Shader.h>

namespace tactics {

class EntityComponentSystem;

namespace renderstep {

class DrawDebug : public RenderStep {
public:
	DrawDebug(EntityComponentSystem& ecs, resource::Shader::Ptr shader);
	void execute(RenderStepInfo& renderInfo) override;

private:
	void _generateUnitSphereLines();
	void _generateUnitBoxLines();

	EntityComponentSystem& _ecs;
	resource::Shader::Ptr _shader;

	struct LineVertex {
		glm::vec3 position;
		glm::vec4 color;
	};

	TVertexBuffer<LineVertex> _vb;
	VertexAttributes _va;
	std::vector<LineVertex> _vertices;
	std::vector<std::pair<glm::vec3, glm::vec3>> _unitSphereLines;
	std::vector<std::pair<glm::vec3, glm::vec3>> _unitBoxLines;
};

} // namespace renderstep
} // namespace tactics
