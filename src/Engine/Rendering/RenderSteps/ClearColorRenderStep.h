#pragma once

#include "RenderStep.h"

namespace tactics::renderstep {

class ClearColor: public RenderStep {
public:
	void render() override;
};

}