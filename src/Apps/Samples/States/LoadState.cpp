#include "LoadState.h"

#include <Engine/Core/RenderSteps/DrawMeshesRenderStep.h>
#include <Engine/Core/RenderSteps/ImGuiRenderSteps.h>
#include <Engine/Core/RenderSteps/PrepareViewportRenderStep.h>
#include <Engine/Scene/SceneSystem.h>

#include <Libs/Ecs/EntityComponentSystem.h>
#include <Libs/Input/InputSystem.h>
#include <Libs/Rendering/RenderQueue.h>
#include <Libs/Rendering/RenderSystem.h>
#include <Libs/Resource/Input/InputMap.h>
#include <Libs/Resource/ResourceSystem.h>

namespace tactics {

LoadState::LoadState(ServiceLocator& services, std::string resourceDefinitionPath, const HashId& resourcePackName)
	: FsmStateWithServices(services)
	, _resourceDefinitionPath(std::move(resourceDefinitionPath))
	, _resourcePackName(resourcePackName) {}

FsmAction LoadState::enter() {
	_loadResources();
	return FsmAction::transition("proceed"_id);
}

void LoadState::exit() {}

FsmAction LoadState::update() {
	return FsmAction::none();
}

void LoadState::_loadResources() {
	auto& resourceSystem = getService<resource::ResourceSystem>();
	resourceSystem.loadPackDefinition(_resourceDefinitionPath);
	resourceSystem.loadPack(_resourcePackName);
}

} // namespace tactics
