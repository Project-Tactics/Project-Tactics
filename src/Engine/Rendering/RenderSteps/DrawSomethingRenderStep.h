#pragma once

#include "RenderStep.h"

#include <Engine/Resource/Shader/Shader.h>

#include <memory>

namespace tactics::renderstep {

class DrawSomething: public RenderStep {
public:
	DrawSomething(std::shared_ptr<Shader> shader);
	void render() override;

private:
	std::shared_ptr<Shader> _shader;
};

}
