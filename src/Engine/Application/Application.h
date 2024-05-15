#pragma once

#include <memory>

struct SDL_Window;

namespace tactics {

class EventsSystem;
class Fsm;
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
	void _initializeFsm();
	void _internalRun();
	void _shutdown();

	std::unique_ptr<RenderSystem> _renderSystem;
	std::unique_ptr<EventsSystem> _eventsSystem;
	std::unique_ptr<Fsm> _fsm;
};

}
