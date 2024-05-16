#include "StartState.h"

#include <Libs/Overlay/OverlaySystem.h>

#include <Engine/Rendering/RenderSystem.h>
#include <Engine/Rendering/Renderer.h>
#include <Engine/Rendering/RenderSteps/ClearColorRenderStep.h>
#include <Engine/Rendering/RenderSteps/DrawSomethingRenderStep.h>
#include <Engine/Rendering/RenderSteps/ImGuiRenderSteps.h>

namespace tactics {

StartState::StartState(RenderSystem& renderSystem, OverlaySystem& overlaySystem)
	: _renderSystem(renderSystem)
	, _overlaySystem(overlaySystem) {
}

FsmAction StartState::enter() {
	using namespace renderstep;

	auto& mainRenderer = _renderSystem.createRenderer();
	mainRenderer.addStep<ClearColor>();
	mainRenderer.addStep<DrawSomething>();

	auto& imguiRenderer = _renderSystem.createRenderer();
	imguiRenderer.addStep<ImGuiBegin>();
	imguiRenderer.addStep<ImGuiRender>(_overlaySystem);
	imguiRenderer.addStep<ImGuiEnd>();

	return FsmAction::transition("proceed");
}

void StartState::exit() {
}

FsmAction StartState::update() {
	return FsmAction::none();
}

}
