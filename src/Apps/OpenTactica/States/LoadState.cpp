#include "LoadState.h"

#include <Engine/Core/RenderSteps/DrawMeshesRenderStep.h>
#include <Engine/Core/RenderSteps/ImGuiRenderSteps.h>
#include <Engine/Core/RenderSteps/PrepareViewportRenderStep.h>
#include <Engine/Scene/SceneSystem.h>

#include <Libs/Ecs/Component/ViewportComponent.h>
#include <Libs/Ecs/EntityComponentSystem.h>
#include <Libs/Rendering/RenderQueue.h>
#include <Libs/Rendering/RenderSystem.h>
#include <Libs/Resource/ResourceSystem.h>

namespace tactics {

LoadState::LoadState(ServiceLocator& services,
					 std::string resourceDefinitionPath,
					 const HashId& resourcePackName,
					 const HashId& cameraPrefab)
	: FsmStateWithServices(services)
	, _resourceDefinitionPath(std::move(resourceDefinitionPath))
	, _resourcePackName(resourcePackName)
	, _cameraPrefab(cameraPrefab) {}

FsmAction LoadState::enter() {
	_loadResources();
	_createViewport();
	_createCamera();
	_setupRenderSteps();
	return FsmAction::transition("proceed"_id);
}

void LoadState::exit() {}

FsmAction LoadState::update() {
	return FsmAction::none();
}

void LoadState::_loadResources() {
	auto& resourceSystem = getService<resource::ResourceSystem>();
	resourceSystem.loadPackDefinition("common/resources.json");
	resourceSystem.loadPackDefinition(_resourceDefinitionPath);
	resourceSystem.loadPack("common"_id);
	resourceSystem.loadPack(_resourcePackName);
}

void LoadState::_createViewport() {
	auto& renderSystem = getService<RenderSystem>();
	auto& sceneSystem = getService<SceneSystem>();
	auto mainViewport = sceneSystem.createViewport({0, 0}, renderSystem.getWindowSize());
	mainViewport.addComponent<component::CurrentViewport>();
}

void LoadState::_createCamera() {
	auto& sceneSystem = getService<SceneSystem>();
	sceneSystem.createEntity("camera"_id, _cameraPrefab);
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

} // namespace tactics
