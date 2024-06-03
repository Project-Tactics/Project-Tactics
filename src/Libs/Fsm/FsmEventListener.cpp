#include "FsmEventListener.h"

namespace tactics {

FsmEventAction FsmEventListener::onEvent(SDL_Event& event) {
	switch (event.type) {
	case SDL_KEYDOWN: {
		return onKeyPress(event.key);
	}
	case SDL_KEYUP: {
		return onKeyRelease(event.key);
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
