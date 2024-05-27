#include "Engine.h"

#include "Application.h"

#include <Engine/ECS/EcsSystem.h>
#include <Engine/Rendering/RenderSystem.h>
#include <Engine/Resource/ResourceSystemInitializer.h>

#include <Libs/Event/EventsSystem.h>
#include <Libs/Fsm/FsmBuilder.h>
#include <Libs/Overlay/OverlaySystem.h>
#include <Libs/Resource/ResourceSystem.h>
#include <Libs/Resource/IniFile/IniFile.h>
#include <Libs/Utility/Service/ServiceLocator.h>
#include <Libs/Utility/Exception.h>

#include <imgui/imgui.h>
#include <SDL.h>

namespace tactics {

void Engine::_run(Application& application) {
	try {
		auto engine = Engine();
		engine._initialize(application);
		engine._internalRun();
		engine._shutdown();
	}
	catch (Exception& exception) {
		printf("%s", exception.what());
	}
	catch (std::exception& exception) {
		printf("%s", exception.what());
	}
}

void Engine::_initialize(Application& application) {
	_initializeSDL();

	_resourceSystem = resource::ResourceSystemInitializer::initialize();

	auto debugConfigFile = _resourceSystem->getResource<resource::IniFile>("devConfigFile");
	_overlaySystem = std::make_unique<OverlaySystem>(debugConfigFile, _resourceSystem->getResourcePathHelper());
	_overlaySystem->setEnabled(true);

	auto configFile = _resourceSystem->getResource<resource::IniFile>("configFile");
	_renderSystem = std::make_unique<RenderSystem>(configFile);
	_resourceSystem->loadResourcePack("builtinMeshes");

	_eventsSystem = std::make_unique<EventsSystem>();
	_ecsSystem = std::make_unique<EcsSystem>();

	_setupServiceLocator();
	_setupFsm(application);
}

void Engine::_internalRun() {
	while (!_fsm->hasReachedExitState()) {
		auto eventResult = _eventsSystem->update();
		if (eventResult == EventResult::QuitGame) {
			return;
		}

		_fsm->update();

		_renderSystem->render();
	}
}

void Engine::_shutdown() {
	_eventsSystem->unregisterEventsListener(_fsm.get());
	_renderSystem.reset();
	_overlaySystem.reset();
	_resourceSystem->unloadResourcePack("initialization");
	_resourceSystem->unloadResourcePack("builtinMeshes");
	_throwIfAnyResourceIsStillLoaded();
	SDL_Quit();
}

void Engine::_throwIfAnyResourceIsStillLoaded() {
	_resourceSystem->forEachResource([] (auto& resource) {
		throw Exception(
			"Resource [{}]:[{}] of type [{}] was not unloaded.",
			resource.name,
			resource.id,
			resource::ResourceTypeSerialization::toString(resource.type));
	});
}

void Engine::_initializeSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw Exception("SDL could not initialize! SDL_Error: {}\n", SDL_GetError());
	}
}

void Engine::_setupFsm(Application& application) {
	auto builder = FsmBuilder();
	auto fsmStartingStateName = application.initialize(*_serviceLocator, builder);
	if (fsmStartingStateName.empty()) {
		throw Exception("Application did not return a valid name for the starting state for the FSM. The name is empty");
	}
	_fsm = builder.build(fsmStartingStateName);
	_eventsSystem->registerEventsListener(_fsm.get());
}

void Engine::_setupServiceLocator() {
	_serviceLocator = std::make_unique<ServiceLocator>();
	_serviceLocator->addService(_resourceSystem.get());
	_serviceLocator->addService(_overlaySystem.get());
	_serviceLocator->addService(_renderSystem.get());
	_serviceLocator->addService(_eventsSystem.get());
	_serviceLocator->addService(_ecsSystem.get());
}

}
