#pragma once

#include <memory>

struct SDL_Window;

namespace tactics {

class EventsSystem;
class Fsm;
class OverlaySystem;
class RenderSystem;
class ResourceSystem;

class Application {
public:
	static void run();

private:
	Application();
	~Application();

	void _initialize();
	void _initializeSDL();
	void _initializeImGui();
	void _initializeFsm();
	void _internalRun();
	void _shutdown();

	std::unique_ptr<ResourceSystem> _resourceSystem;
	std::unique_ptr<RenderSystem> _renderSystem;
	std::unique_ptr<EventsSystem> _eventsSystem;
	std::unique_ptr<OverlaySystem> _overlaySystem;
	std::unique_ptr<Fsm> _fsm;
};

}
