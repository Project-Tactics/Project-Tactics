#include "EventsSystem.h"
#include "EventsListener.h"

#include <SDL.h>

#include <ranges>
#include <exception>
#include <format>

#include <imgui/backends/imgui_impl_sdl2.h>

namespace tactics {

void EventsSystem::update() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		ImGui_ImplSDL2_ProcessEvent(&event);
		for (auto& listener : _eventsListeners) {
			listener->onEvent(event);
		}
	}
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
