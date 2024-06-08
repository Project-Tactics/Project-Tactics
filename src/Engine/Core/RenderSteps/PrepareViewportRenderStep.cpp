#include "PrepareViewportRenderStep.h"

#include <Libs/Ecs/EntityComponentSystem.h>
#include <Libs/Ecs/Component/ViewportComponent.h>
#include <Libs/Utility/Exception.h>

#include <glad/gl.h>

namespace tactics::renderstep {

PrepareViewport::PrepareViewport(EntityComponentSystem& ecs): _ecs(ecs) {
}

void PrepareViewport::execute(RenderStepInfo&) {
	auto view = _ecs.view<component::Viewport, component::CurrentViewport>();

	// Probably an overkill to check this but it's better to be safe than sorry
	// We can always remove or put this under a debug flag
	if (view.size_hint() > 1) {
		throw TACTICS_EXCEPTION("There must be exactly one entity with a CurrentViewport component");
	}

	view.each([&] (component::Viewport& viewport) {
		auto& color = viewport.clearColor;
		glViewport(viewport.topLeft.x, viewport.topLeft.y, viewport.size.x, viewport.size.y);
		glScissor(viewport.topLeft.x, viewport.topLeft.y, viewport.size.x, viewport.size.y);
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	});
}

}
