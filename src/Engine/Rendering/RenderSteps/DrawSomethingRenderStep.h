#pragma once

#include "RenderStep.h"

#include <Engine/Resource/Shader/Shader.h>
#include <Engine/Resource/Texture/Texture.h>

#include <memory>

namespace tactics::renderstep {

class DrawSomething: public RenderStep {
public:
	DrawSomething(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture);
	void render() override;

private:
	std::shared_ptr<Shader> _shader;
	std::shared_ptr<Texture> _texture;
};

}
