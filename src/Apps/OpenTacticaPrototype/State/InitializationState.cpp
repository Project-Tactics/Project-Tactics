#include "InitializationState.h"

#include <Engine/Core/RenderSteps/DrawMeshesRenderStep.h>
#include <Engine/Core/RenderSteps/ImGuiRenderSteps.h>
#include <Engine/Core/RenderSteps/PrepareCameraViewportRenderStep.h>

#include <Libs/Input/InputSystem.h>
#include <Libs/Rendering/RenderQueue.h>
#include <Libs/Rendering/RenderSystem.h>
#include <Libs/Resource/ResourceSystem.h>

namespace tactics {

FsmAction InitializationState::enter() {
	auto& resourceSystem = getService<resource::ResourceSystem>();
	resourceSystem.loadPackDefinition("common/resources.json");
	resourceSystem.loadPackDefinition("game/resources.json");
	resourceSystem.loadPack("common"_id);
	resourceSystem.loadPack("game"_id);

	auto& inputSystem = getService<InputSystem>();
	inputSystem.assignInputMap("emptyStateMap");
	inputSystem.assignDevice(click::DeviceType::Keyboard, 0);

	using namespace renderstep;
	auto& particleSystem = getService<ParticleSystem>();
	auto& ecs = getService<EntityComponentSystem>();
	auto& mainRenderQueue = getService<RenderSystem>().createRenderQueue();
	mainRenderQueue.addStep<PrepareCameraViewport>(ecs);
	mainRenderQueue.addStep<DrawMeshes>(ecs, particleSystem, AlphaBlendedFlag::WithoutAlphaBlend);
	mainRenderQueue.addStep<DrawMeshes>(ecs, particleSystem, AlphaBlendedFlag::WithAlphaBlend);
	mainRenderQueue.addStep<ImGuiRender>(getService<OverlaySystem>());

	return FsmAction::transition("proceed"_id);
}

void InitializationState::exit() {}

FsmAction InitializationState::update() {
	return FsmAction::none();
}

} // namespace tactics
