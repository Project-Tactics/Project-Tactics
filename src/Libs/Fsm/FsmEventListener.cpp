#include "FsmEventListener.h"

namespace tactics {

std::string FsmEventListener::appExitRequestTransition = "_appExitRequest";

FsmEventAction FsmEventListener::onEvent(SDL_Event& event) {
	switch (event.type) {
	case SDL_KEYDOWN: {
		return onKeyPress(event.key);
	}
	case SDL_KEYUP: {
		return onKeyRelease(event.key);
	}
	case SDL_QUIT: {
		return FsmEventAction::transition(appExitRequestTransition);
	}
	}
	return FsmEventAction::none();
}

FsmEventAction FsmEventListener::onKeyPress(SDL_KeyboardEvent&) {
	return FsmEventAction::none();
}

FsmEventAction FsmEventListener::onKeyRelease(SDL_KeyboardEvent&) {
	return FsmEventAction::none();
}

}
