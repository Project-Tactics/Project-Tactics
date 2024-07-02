#include "InputSystem.h"

#include "Click/Backends/SDLClickBackend.h"

#include <Libs/Utility/HashId.h>
#include <Libs/Utility/Log/Log.h>

namespace tactics {

InputSystem::InputSystem() {
	LOG_TRACE(Log::Input, "Initialize click library");
	click::initializeContext();

	_moveUpActionId = click::createAction(click::ActionType::Impulse, "MoveUp");
	click::mapAction<click::DeviceType::Keyboard>(_moveUpActionId,
												  {click::KeyboardAction::KeyW},
												  click::ActionTrigger::Press);
}

void InputSystem::processEvents(SDL_Event& event) { click::processSdlEvents(event); }

void InputSystem::update() {
	if (click::inputState(_moveUpActionId) == click::InputState::Triggered) {
		LOG_INFO(Log::Input, "Move up");
	} else {
		LOG_INFO(Log::Input, "Stop moving up");
	}
}
} // namespace tactics
