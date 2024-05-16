#pragma once

#include "RenderStep.h"

namespace tactics::renderstep {

class DrawSomething: public RenderStep {
public:
	void render() override;
};

}
