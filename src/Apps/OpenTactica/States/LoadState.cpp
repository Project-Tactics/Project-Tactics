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
	using namespace renderstep;

	if (_action == Action::Load) {
		auto& resourceSystem = getService<resource::ResourceSystem>();
		resourceSystem.loadPackDefinition("resource_definitions/game_data.json");
		resourceSystem.loadPackDefinition("resource_definitions/map_resources_data.lua");
		resourceSystem.loadPack("mainPackage");
		resourceSystem.loadPack("mapTextures");

		auto& renderSystem = getService<RenderSystem>();
		auto& scene = getService<SceneSystem>();
		auto mainCamera = scene.createCamera("MainCamera", Vector3::zero, Vector3::forward, Vector3::up, 60, 1, 1000);
		mainCamera.addComponent<component::CurrentCamera>();

		auto mainViewport = scene.createViewport({0, 0}, renderSystem.getWindowSize());
		mainViewport.addComponent<component::CurrentViewport>();

		auto& mainRenderQueue = getService<RenderSystem>().createRenderQueue();

		auto& ecs = getService<EntityComponentSystem>();
		mainRenderQueue.addStep<PrepareViewport>(ecs);
		mainRenderQueue.addStep<DrawMeshes>(ecs, AlphaBlendedFlag::WithoutAlphaBlend);
		mainRenderQueue.addStep<DrawMeshes>(ecs, AlphaBlendedFlag::WithAlphaBlend);
		mainRenderQueue.addStep<ImGuiRender>(getService<OverlaySystem>());
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

}
