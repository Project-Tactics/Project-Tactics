#pragma once

#include "RenderStep.h"

namespace tactics::renderstep {

class ClearColor: public RenderStep {
public:
	void execute(Camera& camera) override;
};

}
