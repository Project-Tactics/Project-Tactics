#include "EventsSystem.h"

#include "EventsListener.h"

#include <Libs/Input/InputSystem.h>

#include <SDL.h>
#include <exception>
#include <format>
#include <imgui/backends/imgui_impl_sdl2.h>
#include <ranges>

namespace tactics {

EventsSystem::EventsSystem(InputSystem& inputSystem) : _inputSystem(inputSystem) {}

void EventsSystem::update() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		ImGui_ImplSDL2_ProcessEvent(&event);
		_inputSystem.processEvents(event);
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

} // namespace tactics
