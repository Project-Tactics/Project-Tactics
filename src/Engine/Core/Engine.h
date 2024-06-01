#pragma once

#include <memory>

struct SDL_Window;

namespace tactics {
namespace resource {
class ResourceSystem;
}

class Application;
class EntityComponentSystem;
class EventsSystem;
class FileSystem;
class Fsm;
class OverlaySystem;
class RenderSystem;
class ServiceLocator;
class SceneSystem;

class Engine {
public:
	template<typename TApplication>
	static void run() {
		TApplication application;
		_run(application);
	}

private:
	static void _run(Application& application);
	void _initialize(Application& application);
	void _initializeSDL();
	void _internalRun();
	void _shutdown();
	void _throwIfAnyResourceIsStillLoaded();

	void _setupServiceLocator();
	void _setupFsm(Application& application);

	std::unique_ptr<FileSystem> _fileSystem;
	std::unique_ptr<resource::ResourceSystem> _resourceSystem;
	std::unique_ptr<OverlaySystem> _overlaySystem;
	std::unique_ptr<RenderSystem> _renderSystem;
	std::unique_ptr<EventsSystem> _eventsSystem;
	std::unique_ptr<EntityComponentSystem> _ecsSystem;
	std::unique_ptr<SceneSystem> _sceneSystem;
	std::unique_ptr<Fsm> _fsm;
	std::unique_ptr<ServiceLocator> _serviceLocator;
};

}
