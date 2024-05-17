#pragma once

#include "RenderStep.h"

#include <Engine/Resources/ShaderResource.h>

#include <memory>

namespace tactics::renderstep {

class DrawSomething: public RenderStep {
public:
	DrawSomething(std::shared_ptr<ShaderResource> shader);
	void render() override;

private:
	std::shared_ptr<ShaderResource> _shader;
};

}
