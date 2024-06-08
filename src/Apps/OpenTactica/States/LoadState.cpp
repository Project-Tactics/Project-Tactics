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
		resourceSystem.loadPackDefinition("game_data.json");
		resourceSystem.loadPack("mainPackage");

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

		// Load the textures programmatically so we don't have to manually copy paste a lot of texture names in the json files
		_loadMapTexturesThroughCustomPack();

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

// TODO(Gerark) of course this should disappear in favor of a prefab/scene resource which takes care of loading all the resources
void LoadState::_loadMapTexturesThroughCustomPack() {
	auto& resourceSystem = getService<resource::ResourceSystem>();

	std::array<int, 5> mapTextureCounts = {10, 13, 8, 5, 14};

	resourceSystem.createManualPack("mapTextures");

	for (auto mapIndex = 0; mapIndex < mapTextureCounts.size(); ++mapIndex) {
		std::string mapName = fmt::format("map{:02d}", mapIndex);
		for (int i = 0; i < mapTextureCounts[mapIndex]; i++) {
			std::string path = fmt::format("textures/{}/tex{:02d}.png", mapName, i);
			nlohmann::json descriptor = {
				{"path", path},
				{"useTransparency", i == 6}
			};
			std::string textureName = fmt::format("{}_{:02d}", mapName, i);
			resourceSystem.loadExternalResource<resource::Texture>("mapTextures", textureName, descriptor);
		}

		std::string path = fmt::format("meshes/{}.fbx", mapName);
		nlohmann::json descriptor = {
			{"path", path}
		};
		resourceSystem.loadExternalResource<resource::Mesh>("mapTextures", mapName, descriptor);
	}
}

}
