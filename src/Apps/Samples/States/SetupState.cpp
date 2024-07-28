#include "SetupState.h"

#include <Engine/Core/RenderSteps/DrawMeshesRenderStep.h>
#include <Engine/Core/RenderSteps/ImGuiRenderSteps.h>
#include <Engine/Core/RenderSteps/PrepareViewportRenderStep.h>
#include <Engine/Scene/SceneSystem.h>

#include <Libs/Ecs/Component/ViewportComponent.h>
#include <Libs/Ecs/EntityComponentSystem.h>
#include <Libs/Input/InputSystem.h>
#include <Libs/Rendering/RenderQueue.h>
#include <Libs/Rendering/RenderSystem.h>
#include <Libs/Resource/Input/InputMap.h>
#include <Libs/Resource/ResourceSystem.h>

namespace tactics {

SetupState::SetupState(ServiceLocator& services) : FsmStateWithServices(services) {}

FsmAction SetupState::enter() {
	_createViewport();
	_setupRenderSteps();
	_setupInputMap();
	return FsmAction::transition("proceed"_id);
}

void SetupState::exit() {}

FsmAction SetupState::update() {
	return FsmAction::none();
}

void SetupState::_createViewport() {
	auto& renderSystem = getService<RenderSystem>();
	auto& sceneSystem = getService<SceneSystem>();
	auto mainViewport = sceneSystem.createViewport({0, 0}, renderSystem.getWindowSize());
	mainViewport.addComponent<component::CurrentViewport>();
}

void SetupState::_setupRenderSteps() {
	using namespace renderstep;
	auto& renderSystem = getService<RenderSystem>();
	auto& mainRenderQueue = renderSystem.createRenderQueue();
	auto& ecs = getService<EntityComponentSystem>();
	mainRenderQueue.addStep<PrepareViewport>(ecs);
	mainRenderQueue.addStep<DrawMeshes>(ecs, AlphaBlendedFlag::WithoutAlphaBlend);
	mainRenderQueue.addStep<DrawMeshes>(ecs, AlphaBlendedFlag::WithAlphaBlend);
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
