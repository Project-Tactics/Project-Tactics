#include "EventsSystem.h"
#include "EventsListener.h"

#include <SDL.h>

#include <ranges>
#include <exception>
#include <format>

namespace tactics {

EventResult EventsSystem::update() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return EventResult::QuitGame;
		}

		for (auto& listener : _eventsListeners) {
			listener->onEvent(event);
		}
	}

	return EventResult::None;
}

void EventsSystem::registerEventsListener(EventsListener* listener) {
	if (auto itr = std::ranges::find(_eventsListeners, listener); itr == _eventsListeners.end()) {
		_eventsListeners.push_back(listener);
	} else {
		throw std::exception(std::format("Attempted to add an already registered listener.").c_str());
	}
}

void EventsSystem::unregisterEventsListener(EventsListener* listener) {
	if (auto itr = std::ranges::find(_eventsListeners, listener); itr != _eventsListeners.end()) {
		_eventsListeners.erase(itr);
	} else {
		throw std::exception(std::format("Attempted to add an already registered listener.").c_str());
	}
}

}
