#include "SetupState.h"

#include <Engine/Core/RenderSteps/DrawMeshesRenderStep.h>
#include <Engine/Core/RenderSteps/DrawUiRenderStep.h>
#include <Engine/Core/RenderSteps/ImGuiRenderSteps.h>
#include <Engine/Core/RenderSteps/PrepareCameraViewportRenderStep.h>
#include <Engine/Scene/SceneSystem.h>

#include <Libs/Ecs/EntityComponentSystem.h>
#include <Libs/Input/InputSystem.h>
#include <Libs/Rendering/RenderQueue.h>
#include <Libs/Rendering/RenderSystem.h>
#include <Libs/Resource/Input/InputMap.h>
#include <Libs/Resource/ResourceSystem.h>

namespace tactics {

SetupState::SetupState(ServiceLocator& services) : FsmStateWithServices(services) {}

FsmAction SetupState::enter() {
	_setupRenderSteps();
	_setupInputMap();
	return FsmAction::transition("proceed"_id);
}

void SetupState::exit() {}

FsmAction SetupState::update() {
	return FsmAction::none();
}

void SetupState::_setupRenderSteps() {
	using namespace renderstep;
	auto& particleSystem = getService<ParticleSystem>();
	auto& ecs = getService<EntityComponentSystem>();
	auto& mainRenderQueue = getService<RenderSystem>().createRenderQueue();
	// auto& uiSystem = getService<jab::UiSystem>();
	// auto& resourceSystem = getService<resource::ResourceSystem>();
	// auto shader = resourceSystem.getResource<resource::Shader>("uiShader"_id);

	mainRenderQueue.addStep<PrepareCameraViewport>(ecs);
	mainRenderQueue.addStep<DrawMeshes>(ecs, particleSystem, AlphaBlendedFlag::WithoutAlphaBlend);
	mainRenderQueue.addStep<DrawMeshes>(ecs, particleSystem, AlphaBlendedFlag::WithAlphaBlend);
	// mainRenderQueue.addStep<DrawUi>(uiSystem, shader);
	mainRenderQueue.addStep<ImGuiRender>(getService<OverlaySystem>());
}

void SetupState::_setupInputMap() {
	auto& inputSystem = getService<InputSystem>();
	auto& resourceSystem = getService<resource::ResourceSystem>();

	auto inputMap = resourceSystem.getResource<resource::InputMap>("commonMap"_id);
	inputSystem.assignInputMap(inputMap, 0);
	inputSystem.assignDevice(click::DeviceType::Keyboard, 0, 0);
	inputSystem.assignDevice(click::DeviceType::Gamepad, 0, 0);
	inputSystem.assignDevice(click::DeviceType::Mouse, 0, 0);
}

} // namespace tactics
