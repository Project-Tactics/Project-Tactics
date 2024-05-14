#pragma once

#include <memory>

struct SDL_Window;

namespace tactics::libs {
class EventsSystem;
}

namespace tactics::engine {

class RenderSystem;

class Application {
public:
	Application();
	~Application();

	void run();

private:
	void _initialize();
	void _initializeSDL();
	void _createWindow();
	void _initializeRenderSystem();
	void _initializeEventsSystem();
	void _internalRun();
	void _shutdown();

	SDL_Window* _window{};
	std::unique_ptr<RenderSystem> _renderSystem;
	std::unique_ptr<libs::EventsSystem> _eventsSystem;
};

}
