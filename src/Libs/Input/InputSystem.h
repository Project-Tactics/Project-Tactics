#pragma once

#include "Click/Click.h"

#include <SDL.h>

namespace tactics {

class InputSystem {
public:
	InputSystem();

	void processEvents(SDL_Event& event);
	void update();

private:
	click::InputActionId _moveActionId;
	click::InputActionId _mouseActionId;
	click::InputActionId _spaceActionId;
};

} // namespace tactics
