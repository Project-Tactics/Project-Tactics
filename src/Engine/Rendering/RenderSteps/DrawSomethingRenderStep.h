#pragma once

#include "RenderStep.h"

#include <Engine/Resource/Shader/Shader.h>
#include <Engine/Resource/Texture/Texture.h>
#include <Engine/Resource/Mesh/Mesh.h>

#include <memory>

namespace tactics::renderstep {

class DrawSomething: public RenderStep {
public:
	DrawSomething(Shader& shader, Texture& texture, Mesh& mesh);
	void execute(RenderStepInfo& renderInfo) override;

private:
	Shader& _shader;
	Texture& _texture;
	Mesh& _mesh;
};

}
