#pragma once

#include <memory>

struct SDL_Window;

namespace tactics {
namespace resource {
class ResourceSystem;
}

class EcsSystem;
class EventsSystem;
class Fsm;
class OverlaySystem;
class RenderSystem;
class ServiceLocator;

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

	std::unique_ptr<resource::ResourceSystem> _resourceSystem;
	std::unique_ptr<RenderSystem> _renderSystem;
	std::unique_ptr<EventsSystem> _eventsSystem;
	std::unique_ptr<OverlaySystem> _overlaySystem;
	std::unique_ptr<EcsSystem> _ecsSystem;
	std::unique_ptr<Fsm> _fsm;
	std::unique_ptr<ServiceLocator> _serviceLocator;
};

}
