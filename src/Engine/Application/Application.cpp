#include "Application.h"

#include <Engine/Rendering/RenderSystem.h>
#include <Libs/Events/EventsSystem.h>

#include <SDL.h>
#include <exception>
#include <format>

namespace tactics::engine {

Application::Application() {
}

Application::~Application() {

}

void Application::run() {
	try {
		_initialize();
		_internalRun();
		_shutdown();
	}
	catch (std::exception& exception) {
		printf(exception.what());
	}
}

void Application::_initialize() {
	_initializeSDL();
	_createWindow();
	_initializeRenderSystem();
	_initializeEventsSystem();
}

void Application::_internalRun() {
	while (1) {
		auto eventResult = _eventsSystem->update();
		if (eventResult == libs::EventResult::QuitGame) {
			return;
		}
		_renderSystem->beginDraw();
		_renderSystem->endDraw();
	}
}

void Application::_shutdown() {
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

void Application::_initializeSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw std::exception(std::format("SDL could not initialize! SDL_Error: {}\n", SDL_GetError()).c_str());
	}
}

void Application::_createWindow() {
	_window = SDL_CreateWindow("Project Tactics", 500, 500, 640, 480, SDL_WINDOW_OPENGL);
	if (_window == nullptr) {
		throw std::exception(std::format("Failed to open window: %s\n", SDL_GetError()).c_str());
	}
}

void Application::_initializeRenderSystem() {
	_renderSystem = std::make_unique<RenderSystem>(_window);
}

void Application::_initializeEventsSystem() {
	_eventsSystem = std::make_unique<libs::EventsSystem>();
}

}
