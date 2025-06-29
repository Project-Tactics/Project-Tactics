#include "UnloadState.h"

#include <Engine/Scene/SceneSystem.h>

#include <Libs/Rendering/RenderSystem.h>
#include <Libs/Resource/ResourceSystem.h>

namespace tactics {

UnloadState::UnloadState(ServiceLocator& services, const HashId& packageName, bool destroyRenderQueue)
	: FsmStateWithServices(services)
	, _packageName(packageName)
	, _destroyRenderQueue(destroyRenderQueue) {}

FsmAction UnloadState::enter() {
	auto& sceneSystem = getService<SceneSystem>();
	sceneSystem.clearScene(true);

	if (_destroyRenderQueue) {
		auto& renderingSystem = getService<RenderSystem>();
		renderingSystem.destroyRenderQueues();
	}

	auto& resourceSystem = getService<resource::ResourceSystem>();
	resourceSystem.unloadPack(_packageName);
	resourceSystem.removePack(_packageName);
	return FsmAction::transition("proceed"_id);
}

FsmAction UnloadState::update() {
	return FsmAction::none();
}

void UnloadState::exit() {}

} // namespace tactics
