#include "##APP_NAME##State.h"

#include <Libs/Input/InputSystem.h>
#include <Libs/Resource/ResourceSystem.h>

namespace tactics {

FsmAction ##APP_NAME##State::enter() {
	auto& resourceSystem = getService<resource::ResourceSystem>();
	resourceSystem.loadPackDefinition("common/resources.json");
	resourceSystem.loadPack("common"_id);

	auto& inputSystem = getService<InputSystem>();
	inputSystem.assignInputMap("emptyStateMap");
	inputSystem.assignDevice(click::DeviceType::Keyboard, 0);
	return FsmAction::none();
}

void ##APP_NAME##State::exit() {
	auto& resourceSystem = getService<resource::ResourceSystem>();
	resourceSystem.unloadPack("common"_id);
}

FsmAction ##APP_NAME##State::update() {
	auto& inputSystem = getService<InputSystem>();
	if (inputSystem.checkAction("anyKeyPressed")) {
		return FsmAction::transition("proceed"_id);
	}

	return FsmAction::none();
}

} // namespace tactics
