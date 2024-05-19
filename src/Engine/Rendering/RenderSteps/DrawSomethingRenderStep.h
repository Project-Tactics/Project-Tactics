#pragma once

#include "RenderStep.h"

#include <Engine/Resource/Shader/Shader.h>
#include <Engine/Resource/Texture/Texture.h>

#include <memory>

namespace tactics::renderstep {

class DrawSomething: public RenderStep {
public:
	DrawSomething(Shader* shader, Texture* texture);
	void execute(Camera& camera) override;

private:
	Shader* _shader;
	Texture* _texture;
};

}
