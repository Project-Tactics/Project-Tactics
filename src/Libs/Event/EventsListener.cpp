#include "EventsListener.h"

namespace tactics {

bool EventsListener::onEvent(SDL_Event& event) {
	switch (event.type) {
	case SDL_KEYDOWN: {
		return onKeyPress(event.key);
	}
	case SDL_KEYUP: {
		return onKeyRelease(event.key);
	}
	}
	return false;
}

bool EventsListener::onKeyPress(SDL_KeyboardEvent& /*event*/) {
	return false;
}

bool EventsListener::onKeyRelease(SDL_KeyboardEvent& /*event*/) {
	return false;
}

} // namespace tactics
