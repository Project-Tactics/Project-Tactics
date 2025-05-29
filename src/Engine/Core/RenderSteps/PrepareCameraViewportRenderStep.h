#pragma once

#include <Libs/Rendering/RenderStep/RenderStep.h>

namespace tactics {
class EntityComponentSystem;
}

namespace tactics::renderstep {

class PrepareCameraViewport : public RenderStep {
public:
	PrepareCameraViewport(EntityComponentSystem& ecs);
	void execute(RenderStepInfo& renderInfo) override;

private:
	EntityComponentSystem& _ecs;
};

} // namespace tactics::renderstep
