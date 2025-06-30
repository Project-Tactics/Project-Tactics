#include "PrepareCameraViewportRenderStep.h"

#include <Libs/Ecs/EntityComponentSystem.h>
#include <Libs/Utility/Exception.h>

#include <glad/gl.h>

namespace tactics::renderstep {

PrepareCameraViewport::PrepareCameraViewport(EntityComponentSystem& ecs) : _ecs(ecs) {}

void PrepareCameraViewport::execute(RenderStepInfo& info) {
	using namespace component;

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

	const auto& registry = _ecs.sceneRegistry();
	auto view = registry.view<Camera, Transform, CurrentCamera>();
	if (view.size_hint() == 0) {
		return;
	}

	auto&& [cameraEntity, camera, cameraTransform] = *view.each().begin();
	info.projection = camera.projection;
	info.view = camera.view;
	info.viewProjection = info.projection * info.view;
	info.cameraPosition = cameraTransform.getPosition();
	info.cameraForward = cameraTransform.getRotation() * Vector3::forward;
	info.cameraForward = glm::normalize(info.cameraForward);
}

} // namespace tactics::renderstep
