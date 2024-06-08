#include "UnloadState.h"

#include <Libs/Resource/ResourceSystem.h>

namespace tactics {

FsmAction UnloadState::enter() {
	auto& resourceSystem = getService<resource::ResourceSystem>();
	resourceSystem.unloadPack("mainPackage");
	resourceSystem.unloadPack("mapTextures");
	return FsmAction::transition("proceed");
}

FsmAction UnloadState::update() {
	return FsmAction::none();
}

void UnloadState::exit() {
}

}
