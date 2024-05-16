#pragma once

#include "RenderStep.h"

namespace tactics {

class ClearColorRenderStep: public RenderStep {
public:
	void render() override;
};

}
