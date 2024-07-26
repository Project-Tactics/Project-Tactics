#pragma once

#include "FsmAction.h"

#include <SDL_events.h>

namespace tactics {

class FsmEventListener {
public:
	FsmEventAction onEvent(SDL_Event& event);

	const static HashId appExitRequestTransition;
};

} // namespace tactics
