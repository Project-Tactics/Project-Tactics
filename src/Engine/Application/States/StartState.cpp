#include "StartState.h"

#include <Libs/Overlay/OverlaySystem.h>

#include <Engine/Rendering/RenderSystem.h>
#include <Engine/Rendering/RenderQueue.h>
#include <Engine/Rendering/RenderSteps/ClearColorRenderStep.h>
#include <Engine/Rendering/RenderSteps/DrawSomethingRenderStep.h>
#include <Engine/Rendering/RenderSteps/ImGuiRenderSteps.h>

#include <Engine/Resource/ResourceSystem.h>

namespace tactics {

StartState::StartState(ResourceSystem& resourceSystem, RenderSystem& renderSystem, OverlaySystem& overlaySystem)
	: _resourceSystem(resourceSystem)
	, _renderSystem(renderSystem)
	, _overlaySystem(overlaySystem) {
}

FsmAction StartState::enter() {
	using namespace renderstep;

	_resourceSystem.loadResourcePackDefinition("resources.lua");
	_resourceSystem.loadResourcePack("mainPackage");

	auto shader = _resourceSystem.getResource<Shader>("main");

	auto& mainRenderQueue = _renderSystem.createRenderQueue();
	mainRenderQueue.addStep<ClearColor>();
	mainRenderQueue.addStep<DrawSomething>(shader);

	auto& imguiRenderQueue = _renderSystem.createRenderQueue();
	imguiRenderQueue.addStep<ImGuiBegin>();
	imguiRenderQueue.addStep<ImGuiRender>(_overlaySystem);
	imguiRenderQueue.addStep<ImGuiEnd>();

	return FsmAction::transition("proceed");
}

void StartState::exit() {
}

FsmAction StartState::update() {
	return FsmAction::none();
}

}
