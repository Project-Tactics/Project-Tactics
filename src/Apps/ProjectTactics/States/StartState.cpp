#include "StartState.h"

#include <Libs/Overlay/OverlaySystem.h>
#include <Libs/Resource/ResourceSystem.h>

#include <Engine/ECS/EcsSystem.h>
#include <Engine/Rendering/RenderSystem.h>
#include <Engine/Rendering/RenderQueue.h>
#include <Engine/Rendering/RenderSteps/ClearViewportRenderStep.h>
#include <Engine/Rendering/RenderSteps/DrawMeshesRenderStep.h>
#include <Engine/Rendering/RenderSteps/ImGuiRenderSteps.h>

namespace tactics {

FsmAction StartState::enter() {
	using namespace renderstep;

	auto& resourceSystem = getService<resource::ResourceSystem>();
	resourceSystem.loadResourcePackDefinition("game_res.lua");
	resourceSystem.loadResourcePack("mainPackage");

	auto& mainRenderQueue = getService<RenderSystem>().createRenderQueue();
	mainRenderQueue.addStep<ClearViewport>();
	mainRenderQueue.addStep<DrawMeshes>(getService<EcsSystem>(), AlphaBlendedFlag::WithoutAlphaBlend);
	mainRenderQueue.addStep<DrawMeshes>(getService<EcsSystem>(), AlphaBlendedFlag::WithAlphaBlend);

	mainRenderQueue.addStep<ImGuiRender>(getService<OverlaySystem>());

	return FsmAction::transition("proceed");
}

void StartState::exit() {
}

FsmAction StartState::update() {
	return FsmAction::none();
}

}
