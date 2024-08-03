#include "PrepareViewportRenderStep.h"

#include <Libs/Ecs/EntityComponentSystem.h>
#include <Libs/Utility/Exception.h>

#include <glad/gl.h>

namespace tactics::renderstep {

PrepareViewport::PrepareViewport(EntityComponentSystem& ecs) : _ecs(ecs) {}

void PrepareViewport::execute(RenderStepInfo& info) {
	auto& color = info.viewport.clearColor;
	auto& size = info.viewport.size;
	auto& pos = info.viewport.position;
	auto x = static_cast<unsigned int>(pos.x * info.windowSize.x);
	auto y = static_cast<unsigned int>(pos.y * info.windowSize.y);
	auto width = static_cast<unsigned int>(size.x * info.windowSize.x);
	auto height = static_cast<unsigned int>(size.y * info.windowSize.y);
	glViewport(x, y, width, height);
	glScissor(x, y, width, height);
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

} // namespace tactics::renderstep
