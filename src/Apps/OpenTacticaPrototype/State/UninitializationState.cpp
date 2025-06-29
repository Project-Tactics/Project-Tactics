#include "UninitializationState.h"

#include <Libs/Resource/ResourceSystem.h>

namespace tactics {

FsmAction UninitializationState::enter() {
	auto& resourceSystem = getService<resource::ResourceSystem>();
	resourceSystem.unloadPack("game"_id);
	resourceSystem.unloadPack("common"_id);
	return FsmAction::transition("proceed"_id);
}

void UninitializationState::exit() {}

FsmAction UninitializationState::update() {
	return FsmAction::none();
}

} // namespace tactics
