#pragma once

#include <Libs/Rendering/RenderStep/RenderStep.h>

namespace tactics {
class EntityComponentSystem;
}

namespace tactics::renderstep {

class PrepareViewport: public RenderStep {
public:
	PrepareViewport(EntityComponentSystem& ecs);
	void execute(RenderStepInfo& renderInfo) override;

private:
	EntityComponentSystem& _ecs;
};

}
