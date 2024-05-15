#include "Application.h"

#include "States/MapState.h"
#include "States/StartState.h"

#include <Engine/Rendering/RenderSystem.h>
#include <Libs/Events/EventsSystem.h>
#include <Libs/Fsm/FsmBuilder.h>
#include <Libs/Overlay/OverlaySystem.h>
#include <Libs/Overlay/ExampleOverlay.h>
#include <Libs/Utilities/Exception.h>

#include <SDL.h>
#include <exception>
#include <format>

namespace tactics {

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
	catch (Exception& exception) {
		printf("%s", exception.what());
	}
	catch (std::exception& exception) {
		printf("%s", exception.what());
	}
}

void Application::_initialize() {
	_initializeSDL();
	_initializeRenderSystem();
	_initializeEventsSystem();
	_initializeOverlaySystem();
	_initializeFsm();
}

void Application::_internalRun() {
	while (!_fsm->hasReachedExitState()) {
		auto eventResult = _eventsSystem->update();
		if (eventResult == EventResult::QuitGame) {
			return;
		}
		_renderSystem->beginDraw();
		_fsm->update();

		_renderSystem->beginDrawOverlay();
		_overlaySystem->update();
		_renderSystem->endDrawOverlay();

		_renderSystem->endDraw();
	}
}

void Application::_shutdown() {
	_overlaySystem->removeOverlay("ImGuiDemo");
	_eventsSystem->unregisterEventsListener(_fsm.get());
	SDL_Quit();
}

void Application::_initializeSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw std::exception(std::format("SDL could not initialize! SDL_Error: {}\n", SDL_GetError()).c_str());
	}
}

void Application::_initializeRenderSystem() {
	_renderSystem = std::make_unique<RenderSystem>();
}

void Application::_initializeEventsSystem() {
	_eventsSystem = std::make_unique<EventsSystem>();
}

void Application::_initializeFsm() {
	auto builder = FsmBuilder();

	builder
		.state<StartState>("Start")
		.on("proceed").jumpTo("Map")

		.state<MapState>("Map", *_renderSystem)
		.on("exit").exitFsm();

	_fsm = builder.build("Start");

	_eventsSystem->registerEventsListener(_fsm.get());
}

void Application::_initializeOverlaySystem() {
	_overlaySystem = std::make_unique<OverlaySystem>();
	_overlaySystem->setEnabled(true);
	_overlaySystem->addOverlay("ImGuiDemo", std::make_unique<ExampleOverlay>());
}

}
