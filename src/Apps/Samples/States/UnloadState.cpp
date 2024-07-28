#include "UnloadState.h"

#include <Engine/Scene/SceneSystem.h>

#include <Libs/Resource/ResourceSystem.h>

namespace tactics {

UnloadState::UnloadState(ServiceLocator& services, const HashId& packageName)
	: FsmStateWithServices(services)
	, _packageName(packageName) {}

FsmAction UnloadState::enter() {
	auto& sceneSystem = getService<SceneSystem>();
	sceneSystem.clearScene(true);

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
