#include "Engine.h"

#include "Application.h"
#include "DefaultFsmExternalController.h"
#include "ResourceSystemInitializer.h"

#include <Engine/Overlay/FsmOverlay.h>
#include <Engine/Overlay/RenderingOverlay.h>
#include <Engine/Overlay/ResourcesOverlay.h>
#include <Engine/Overlay/CustomOverlayColors.h>
#include <Engine/Scene/SceneSystem.h>

#include <Libs/Ecs/EntityComponentSystem.h>
#include <Libs/Ecs/System/CameraSystem.h>
#include <Libs/Ecs/System/TransformSystem.h>
#include <Libs/Ecs/System/BillboardSystem.h>
#include <Libs/Ecs/System/SpriteSystem.h>
#include <Libs/Event/EventsSystem.h>
#include <Libs/FileSystem/FileSystem.h>
#include <Libs/FileSystem/FileLoader.h>
#include <Libs/Fsm/FsmBuilder.h>
#include <Libs/Overlay/OverlaySystem.h>
#include <Libs/Overlay/MainOverlay.h>
#include <Libs/Overlay/ExampleOverlay.h>
#include <Libs/Rendering/RenderSystem.h>
#include <Libs/Resource/IniFile/IniFile.h>
#include <Libs/Resource/ResourceSystem.h>
#include <Libs/Resource/Texture/Texture.h>
#include <Libs/Utility/Exception.h>
#include <Libs/Utility/Log/Log.h>
#include <Libs/Utility/Math.h>
#include <Libs/Utility/Service/ServiceLocator.h>

#include <imgui/imgui.h>
#include <SDL.h>

namespace tactics {

void Engine::_run(Application& application) {
	try {
		Log::init(LogLevel::Trace);
		LOG_TRACE(Log::Engine, "Engine Initialization Started");
		auto engine = Engine();
		engine._initialize(application);
		LOG_INFO(Log::Engine, "Engine Initialized");
		engine._internalRun();
		LOG_TRACE(Log::Engine, "Engine Shutdown Started");
		engine._shutdown();
		LOG_TRACE(Log::Engine, "Engine Shutdown Ended");
	}
	catch (Exception& exception) {
		LOG_EXCEPTION(exception);
	}
	catch (std::exception& exception) {
		LOG_EXCEPTION(exception);
	}
}

void Engine::_initialize(Application& application) {
	_initializeSDL();

	LOG_TRACE(Log::Engine, "FileSystem Initialization");
	auto pathHelper = std::make_unique<PathHelper>("data");
	auto fileLoader = std::make_unique<DefaultFileLoader>(*pathHelper.get());
	_fileSystem = std::make_unique<FileSystem>(std::move(fileLoader), std::move(pathHelper));

	LOG_TRACE(Log::Engine, "EntityComponentSystem Initialization");
	_ecs = std::make_unique<EntityComponentSystem>();

	LOG_TRACE(Log::Engine, "ResourceSystem Initialization");
	_resourceSystem = ResourceSystemInitializer::initialize(*_fileSystem, *_ecs);

	LOG_TRACE(Log::Engine, "OverlaySystem Initialization");
	auto devUserConfigFile = _resourceSystem->getResource<resource::IniFile>("devUserConfigFile");
	auto imguiSettings = _resourceSystem->getResource<resource::IniFile>("imguiSettings");
	_overlaySystem = std::make_unique<OverlaySystem>(devUserConfigFile, *imguiSettings, *_fileSystem);
	_overlaySystem->setEnabled(true);
	CustomOverlayColors::initialize(*imguiSettings);

	LOG_TRACE(Log::Engine, "Load Engine Resources");
	auto configFile = _resourceSystem->getResource<resource::IniFile>("configFile");
	_renderSystem = std::make_unique<RenderSystem>(configFile);
	_resourceSystem->loadPack("builtinMeshes");
	_resourceSystem->createManualPack("_internalCustomPack");
	_resourceSystem->loadExternalResource("_internalCustomPack", resource::Texture::createNullTexture());

	LOG_TRACE(Log::Engine, "EventSystem Initialization");
	_eventsSystem = std::make_unique<EventsSystem>();

	LOG_TRACE(Log::Engine, "EventSystem SceneSystem");
	_sceneSystem = std::make_unique<SceneSystem>(*_ecs, *_resourceSystem);

	_setupServiceLocator();

	application.setupComponentReflections();
	_setupFsm(application);

	_registerOverlays();
}

void Engine::_internalRun() {
	while (!_fsm->hasReachedExitState()) {
		_eventsSystem->update();
		_fsm->update();
		_updateCommonComponentSystems();
		_renderSystem->render();
	}
}

void Engine::_shutdown() {
	_unregisterOverlays();
	_eventsSystem->unregisterEventsListener(_fsm.get());
	_renderSystem.reset();
	_overlaySystem.reset();
	_ecs->clearPrefabsRegistry();
	LOG_TRACE(Log::Engine, "Unload Engine Resources");
	_resourceSystem->unloadPack("initialization");
	_resourceSystem->unloadPack("builtinMeshes");
	_resourceSystem->unloadPack("_internalCustomPack");
	_throwIfAnyResourceIsStillLoaded();
	_throwIfAnyImportantLogHappened();
	SDL_Quit();
	LOG_TRACE(Log::Engine, "Quit SDL");
}

void Engine::_registerOverlays() {
	auto debugConfigFile = _resourceSystem->getResource<resource::IniFile>("devUserConfigFile");
	if (debugConfigFile->getOrCreate("overlay", "enableEngineOverlay", false)) {
		LOG_TRACE(Log::Engine, "Register Engine Overlays");
		_overlaySystem->addOverlay<MainOverlay>("Main", true, *_overlaySystem);
		_overlaySystem->addOverlay<RenderingOverlay>("Rendering", false, *_renderSystem, *_ecs);
		_overlaySystem->addOverlay<ResourcesOverlay>("Resources", false, *_resourceSystem);
		_overlaySystem->addOverlay<FsmOverlay>("Fsm", false, *_fsmExternalController, *_fsmInfo);
		_overlaySystem->addOverlay<ExampleOverlay>("ImGui Demo", false);
	}
}

void Engine::_unregisterOverlays() {
	auto debugConfigFile = _resourceSystem->getResource<resource::IniFile>("devUserConfigFile");
	if (debugConfigFile->getOrCreate("overlay", "enableEngineOverlay", false)) {
		LOG_TRACE(Log::Engine, "Unregister Engine Overlays");
		_overlaySystem->removeOverlay("Main");
		_overlaySystem->removeOverlay("Rendering");
		_overlaySystem->removeOverlay("ImGui Demo");
	}
}

void Engine::_throwIfAnyResourceIsStillLoaded() {
	LOG_TRACE(Log::Engine, "Check if any resource is still loaded");
	_resourceSystem->forEachManager([] (auto& manager) {
		manager.forEachResource([] (const auto& resource) {
			throw TACTICS_EXCEPTION(
				"Resource [{}]:[{}] of type [{}] was not unloaded.",
				resource.name,
				resource.id,
				toString(resource.type));
		});
	});
}

void Engine::_throwIfAnyImportantLogHappened() {
	if (Log::hasBeenLoggedOverLevel(LogLevel::Warning)) {
		throw TACTICS_EXCEPTION("Do not ignore logs over warning level.\nRecap:\n{}",
			Log::getLogCountRecapMessage());
	}
}

void Engine::_initializeSDL() {
	LOG_TRACE(Log::Engine, "SDL Initialization Started");
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw TACTICS_EXCEPTION("SDL could not initialize! SDL_Error: {}\n", SDL_GetError());
	}
	LOG_TRACE(Log::Engine, "SDL Initialization Ended");
}

void Engine::_setupFsm(Application& application) {
	LOG_TRACE(Log::Engine, "Building Game FSM");
	auto builder = FsmBuilder();
	auto fsmStartingStateName = application.initialize(*_serviceLocator, builder);
	if (fsmStartingStateName.empty()) {
		throw TACTICS_EXCEPTION("Application did not return a valid name for the starting state for the FSM. The name is empty");
	}

	_fsmExternalController = std::make_unique<DefaultFsmExternalController>();
	std::tie(_fsm, _fsmInfo) = builder.build(fsmStartingStateName, _fsmExternalController.get());
	_eventsSystem->registerEventsListener(_fsm.get());
}

void Engine::_setupServiceLocator() {
	_serviceLocator = std::make_unique<ServiceLocator>();
	_serviceLocator->addService(_resourceSystem.get());
	_serviceLocator->addService(_overlaySystem.get());
	_serviceLocator->addService(_renderSystem.get());
	_serviceLocator->addService(_eventsSystem.get());
	_serviceLocator->addService(_ecs.get());
	_serviceLocator->addService(_sceneSystem.get());
	_serviceLocator->addService(_fileSystem.get());
}

void Engine::_updateCommonComponentSystems() {
	using namespace component;

	auto& registry = _ecs->sceneRegistry();

	SpriteAnimationSystem::update(registry.view<Sprite, SpriteAnimation>());
	SpriteSystem::update(registry.view<Sprite, Mesh>());

	CameraSystem::updateCameraAspectRatios(
		registry.view<Viewport, CurrentViewport>(),
		registry.view<Frustum, CurrentCamera>());

	CameraSystem::updateCameraMatrices(registry.view<Frustum, Transform, Camera>());

	BillboardSystem::update(
		registry.view<Transform, Camera, CurrentCamera>(),
		registry.view<Transform, Billboard>());

	TransformSystem::updateTransformMatrices(registry.view<Transform>());
}

}
