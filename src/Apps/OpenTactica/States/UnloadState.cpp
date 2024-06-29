#include "UnloadState.h"

#include <Libs/Resource/ResourceSystem.h>

namespace tactics {

UnloadState::UnloadState(ServiceLocator& services, const hash_string& packageName)
	: FsmStateWithServices(services)
	, _packageName(packageName) {
}

FsmAction UnloadState::enter() {
	auto& resourceSystem = getService<resource::ResourceSystem>();
	resourceSystem.unloadPack(_packageName);
	resourceSystem.unloadPack("common");
	return FsmAction::transition("proceed");
}

FsmAction UnloadState::update() {
	return FsmAction::none();
}

void UnloadState::exit() {
}

}
