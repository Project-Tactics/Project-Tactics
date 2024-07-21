#pragma once

#include <Libs/Utility/Time/FrameTimer.h>

#include <memory>

struct SDL_Window;

namespace tactics {
namespace resource {
class ResourceSystem;
}

class Application;
class DefaultFsmExternalController;
class EntityComponentSystem;
class EventsSystem;
class FileSystem;
class Fsm;
class InputSystem;
class OverlaySystem;
class RenderSystem;
class ServiceLocator;
class SceneSystem;
struct FsmInfo;

class Engine {
public:
	template<typename TApplication> static void run() {
		TApplication application;
		_run(application);
	}

private:
	static void _run(Application& application);
	void _initialize(Application& application);
	void _initializeSDL();
	void _internalRun();
	void _shutdown();
	void _registerOverlays();
	void _unregisterOverlays();
	void _throwIfAnyResourceIsStillLoaded();
	void _throwIfAnyImportantLogHappened();

	void _setupServiceLocator();
	void _setupFsm(Application& application);

	void _updateCommonComponentSystems();

	FrameTimer _timer;
	std::unique_ptr<FileSystem> _fileSystem;
	std::unique_ptr<resource::ResourceSystem> _resourceSystem;
	std::unique_ptr<InputSystem> _inputSystem;
	std::unique_ptr<OverlaySystem> _overlaySystem;
	std::unique_ptr<RenderSystem> _renderSystem;
	std::unique_ptr<EventsSystem> _eventsSystem;
	std::unique_ptr<EntityComponentSystem> _ecs;
	std::unique_ptr<SceneSystem> _sceneSystem;
	std::unique_ptr<Fsm> _fsm;
	std::unique_ptr<FsmInfo> _fsmInfo;
	std::unique_ptr<ServiceLocator> _serviceLocator;
	std::unique_ptr<DefaultFsmExternalController> _fsmExternalController;
};

} // namespace tactics
