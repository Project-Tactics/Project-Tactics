#include "DefaultState.h"

#include <Libs/Input/InputSystem.h>
#include <Libs/Resource/ResourceSystem.h>

namespace tactics {

FsmAction DefaultState::enter() {
	auto& resourceSystem = getService<resource::ResourceSystem>();
	resourceSystem.loadPackDefinition("common/resources.json");
	resourceSystem.loadPack("common"_id);

	auto& inputSystem = getService<InputSystem>();
	inputSystem.assignInputMap("emptyStateMap");
	inputSystem.assignDevice(click::DeviceType::Keyboard, 0);
	return FsmAction::none();
}

void DefaultState::exit() {
	auto& resourceSystem = getService<resource::ResourceSystem>();
	resourceSystem.unloadPack("common"_id);
}

FsmAction DefaultState::update() {
	auto& inputSystem = getService<InputSystem>();
	if (inputSystem.checkAction("anyKeyPressed")) {
		return FsmAction::transition("proceed"_id);
	}

	return FsmAction::none();
}

} // namespace tactics
