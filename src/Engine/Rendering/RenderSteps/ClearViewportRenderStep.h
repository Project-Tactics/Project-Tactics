#pragma once

#include "RenderStep.h"

namespace tactics::renderstep {

class ClearViewport: public RenderStep {
public:
	void execute(RenderStepInfo& renderInfo) override;
};

}
