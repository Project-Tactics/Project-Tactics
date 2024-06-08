#include "LoadState.h"

#include <Engine/Core/RenderSteps/PrepareViewportRenderStep.h>
#include <Engine/Core/RenderSteps/DrawMeshesRenderStep.h>
#include <Engine/Core/RenderSteps/ImGuiRenderSteps.h>
#include <Engine/Scene/SceneSystem.h>

#include <Libs/Ecs/EntityComponentSystem.h>
#include <Libs/Ecs/Component/CameraComponent.h>
#include <Libs/Ecs/Component/ViewportComponent.h>
#include <Libs/Overlay/OverlaySystem.h>
#include <Libs/Rendering/RenderSystem.h>
#include <Libs/Resource/ResourceSystem.h>
#include <Libs/Resource/Mesh/Mesh.h>
#include <Libs/Rendering/RenderQueue.h>
#include <Libs/Utility/Math.h>

namespace tactics {

LoadState::LoadState(ServiceLocator& services, Action action): FsmStateWithServices(services), _action(action) {
}

FsmAction LoadState::enter() {

	if (_action == Action::Load) {
		_loadResources();
		_createViewport();
		_createCamera();
		_setupRenderSteps();
	} else {
		auto& resourceSystem = getService<resource::ResourceSystem>();
		resourceSystem.unloadPack("mainPackage");
		resourceSystem.unloadPack("mapTextures");
	}

	return FsmAction::transition("proceed");
}

void LoadState::exit() {
}

FsmAction LoadState::update() {
	return FsmAction::none();
}

void LoadState::_loadResources() {
	auto& resourceSystem = getService<resource::ResourceSystem>();
	resourceSystem.loadPackDefinition("resource_definitions/game_data.json");
	resourceSystem.loadPackDefinition("resource_definitions/map_resources_data.lua");
	resourceSystem.loadPack("mainPackage");
	resourceSystem.loadPack("mapTextures");
}

void LoadState::_createViewport() {
	auto& renderSystem = getService<RenderSystem>();
	auto& sceneSystem = getService<SceneSystem>();
	auto mainViewport = sceneSystem.createViewport({0, 0}, renderSystem.getWindowSize());
	mainViewport.addComponent<component::CurrentViewport>();
}

void LoadState::_createCamera() {
	auto& sceneSystem = getService<SceneSystem>();
	sceneSystem.createEntity("camera", "mapCamera");
}

void LoadState::_setupRenderSteps() {
	using namespace renderstep;
	auto& renderSystem = getService<RenderSystem>();
	auto& mainRenderQueue = renderSystem.createRenderQueue();
	auto& ecs = getService<EntityComponentSystem>();
	mainRenderQueue.addStep<PrepareViewport>(ecs);
	mainRenderQueue.addStep<DrawMeshes>(ecs, AlphaBlendedFlag::WithoutAlphaBlend);
	mainRenderQueue.addStep<DrawMeshes>(ecs, AlphaBlendedFlag::WithAlphaBlend);
	mainRenderQueue.addStep<ImGuiRender>(getService<OverlaySystem>());
}

}
