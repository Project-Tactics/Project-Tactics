#include "StartState.h"

#include <Libs/Overlay/OverlaySystem.h>
#include <Libs/Resource/ResourceSystem.h>

#include <Engine/Rendering/RenderSystem.h>
#include <Engine/Rendering/RenderQueue.h>
#include <Engine/Rendering/RenderSteps/ClearViewportRenderStep.h>
#include <Engine/Rendering/RenderSteps/DrawSomethingRenderStep.h>
#include <Engine/Rendering/RenderSteps/ImGuiRenderSteps.h>

namespace tactics {

StartState::StartState(ResourceSystem& resourceSystem, RenderSystem& renderSystem, OverlaySystem& overlaySystem)
	: _resourceSystem(resourceSystem)
	, _renderSystem(renderSystem)
	, _overlaySystem(overlaySystem) {
}

FsmAction StartState::enter() {
	using namespace renderstep;

	_resourceSystem.loadResourcePackDefinition("game_res.lua");
	_resourceSystem.loadResourcePack("mainPackage");

	auto shader = _resourceSystem.getResource<Shader>("main");
	auto texture = _resourceSystem.getResource<Texture>("tactics-icon");

	auto& mainRenderQueue = _renderSystem.createRenderQueue();
	mainRenderQueue.addStep<ClearViewport>();
	mainRenderQueue.addStep<DrawSomething>(shader, texture);

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
