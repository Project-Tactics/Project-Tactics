#pragma once

#include "FsmAction.h"
#include <SDL_events.h>

namespace tactics {

class FsmEventListener {
public:
	FsmEventAction onEvent(SDL_Event& event);

protected:
	virtual FsmEventAction onKeyPress(SDL_KeyboardEvent& event);
	virtual FsmEventAction onKeyRelease(SDL_KeyboardEvent& event);
};

}
