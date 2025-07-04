#include "Engine.h"

#include "Application.h"
#include "DefaultFsmExternalController.h"
#include "ResourceSystemInitializer.h"

#include <Engine/Overlay/CustomOverlayColors.h>
#include <Engine/Overlay/EngineCoreOverlay.h>
#include <Engine/Overlay/FsmOverlay.h>
#include <Engine/Overlay/InputOverlay.h>
#include <Engine/Overlay/RenderingOverlay.h>
#include <Engine/Overlay/ResourcesOverlay.h>
#include <Engine/Scene/SceneSystem.h>

#include <Libs/Ecs/EntityComponentSystem.h>
#include <Libs/Ecs/System/BillboardSystem.h>
#include <Libs/Ecs/System/CameraSystem.h>
#include <Libs/Ecs/System/DebugDrawingSystem.h>
#include <Libs/Ecs/System/SpriteSystem.h>
#include <Libs/Ecs/System/TransformSystem.h>
#include <Libs/Event/EventsSystem.h>
#include <Libs/FileSystem/FileLoader.h>
#include <Libs/FileSystem/FileSystem.h>
#include <Libs/Fsm/FsmBuilder.h>
#include <Libs/Input/InputSystem.h>
#include <Libs/Overlay/ExampleOverlay.h>
#include <Libs/Overlay/MainOverlay.h>
#include <Libs/Overlay/OverlaySystem.h>
#include <Libs/Physics/PhysicsSystem.h>
#include <Libs/Rendering/Particle/ParticleSystem.h>
#include <Libs/Rendering/RenderSystem.h>
#include <Libs/Resource/DataSet/DataSetSystem.h>
#include <Libs/Resource/IniFile/IniFile.h>
#include <Libs/Resource/ResourceSystem.h>
#include <Libs/Resource/Texture/Texture.h>
#include <Libs/UI/UISystem.h>
#include <Libs/Utility/Exception.h>
#include <Libs/Utility/Log/Log.h>
#include <Libs/Utility/Math.h>
#include <Libs/Utility/Service/ServiceLocator.h>
#include <Libs/Utility/Time/EngineTime.h>
#include <Libs/Utility/Time/TimeUtility.h>

#include <SDL.h>
#include <imgui/imgui.h>

namespace tactics {

void Engine::_run(Application& application) {
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

void Engine::_initialize(Application& application) {
	_initializeSDL();

	Random::setInstance(&_random);
	_timer.setFixedDeltaTime(1.0 / 60.0);
	EngineTime::setFrameTime(&_timer);

	LOG_TRACE(Log::Engine, "FileSystem Initialization");
	auto pathHelper = std::make_unique<PathHelper>("data");
	auto fileLoader = std::make_unique<DefaultFileLoader>(*pathHelper.get());
	_fileSystem = std::make_unique<FileSystem>(std::move(fileLoader), std::move(pathHelper));

	LOG_TRACE(Log::Engine, "EntityComponentSystem Initialization");
	_ecs = std::make_unique<EntityComponentSystem>();

	LOG_TRACE(Log::Engine, "ResourceSystem Initialization");
	_resourceSystem = ResourceSystemInitializer::initialize(*_fileSystem, *_ecs);
	_dataSetSystem = std::make_unique<resource::DataSetSystem>(*_resourceSystem);

	LOG_TRACE(Log::Engine, "OverlaySystem Initialization");
	auto devUserConfigFile = _resourceSystem->getResource<resource::IniFile>("devUserConfigFile"_id);
	auto imguiSettings = _resourceSystem->getResource<resource::IniFile>("imguiSettings"_id);
	_overlaySystem = std::make_unique<OverlaySystem>(devUserConfigFile, *imguiSettings, *_fileSystem);
	_overlaySystem->setEnabled(true);
	CustomOverlayColors::initialize(*imguiSettings);

	LOG_TRACE(Log::Engine, "Load Engine Resources");
	auto configFile = _resourceSystem->getResource<resource::IniFile>("configFile"_id);
	_renderSystem = std::make_unique<RenderSystem>(configFile);
	_renderSystem->setViewport({0, 0}, {1, 1}, Color::black);

	LOG_TRACE(Log::Engine, "ParticleSystem Initialization");
	_particleSystem = std::make_unique<ParticleSystem>(*_resourceSystem, *_ecs);

	LOG_TRACE(Log::Engine, "PhysicsSystem Initialization");
	constexpr int tempAllocatorSize = 1 * 1024 * 1024; // Let's allocate 1 MB for temporary allocations
	_physicsSystem = std::make_unique<PhysicsSystem>(tempAllocatorSize, *_ecs);

	_resourceSystem->loadPack("builtinMeshes"_id);
	_resourceSystem->createManualPack("_internalCustomPack"_id);
	_resourceSystem->loadExternalResource("_internalCustomPack"_id, resource::Texture::createNullTexture());

	LOG_TRACE(Log::Engine, "InputSystem Initialization");
	_inputSystem = std::make_unique<InputSystem>(configFile, *_resourceSystem, _renderSystem->getWindowSize());

	LOG_TRACE(Log::Engine, "EventSystem Initialization");
	_eventsSystem = std::make_unique<EventsSystem>(*_inputSystem);
	_eventsSystem->registerEventsListener(_renderSystem.get());

	LOG_TRACE(Log::Engine, "SceneSystem Initialization");
	_sceneSystem = std::make_unique<SceneSystem>(*_ecs, *_resourceSystem);

	LOG_TRACE(Log::Engine, "UiSystem Initialization");
	_uiSystem = std::make_unique<jab::UiSystem>();

	_setupServiceLocator();

	application.setupComponentReflections();
	_setupFsm(application);

	_registerOverlays();
}

void Engine::_internalRun() {
	_timer.reset(TimeUtility::nowInSeconds());
	while (!_fsm->hasReachedExitState()) {
		_timer.update(TimeUtility::nowInSeconds());
		_eventsSystem->update();

		auto ticksProcessed = 0;
		const auto maxTicksPerFrame = 20;

		while (_timer.hasConsumedAllTicks() && ticksProcessed < maxTicksPerFrame) {
			_inputSystem->update();
			_fsm->update();
			_updateCommonComponentSystems();
			_timer.consumeTick();
			++ticksProcessed;
		}

		if (ticksProcessed >= maxTicksPerFrame) {
			LOG_DEBUG(Log::Engine,
					  "Tick cap hit ({} ticks processed). This may be caused by a long frame, a debugging pause, or "
					  "performance issues causing the simulation to fall behind.",
					  ticksProcessed);
		}

		_renderSystem->render();
	}
}

void Engine::_shutdown() {
	_unregisterOverlays();
	_eventsSystem->unregisterEventsListener(_fsmExternalController.get());
	_eventsSystem->unregisterEventsListener(_renderSystem.get());
	_renderSystem.reset();
	_particleSystem.reset();
	_overlaySystem.reset();
	_physicsSystem.reset();
	_uiSystem.reset();
	_ecs->clearPrefabsRegistry();
	LOG_TRACE(Log::Engine, "Unload Engine Resources");
	_resourceSystem->unloadPack("initialization"_id);
	_resourceSystem->unloadPack("builtinMeshes"_id);
	_resourceSystem->unloadPack("_internalCustomPack"_id);
	_throwIfAnyResourceIsStillLoaded();
	_throwIfAnyImportantLogHappened();
	SDL_Quit();
	LOG_TRACE(Log::Engine, "Quit SDL");
}

void Engine::_registerOverlays() {
	auto debugConfigFile = _resourceSystem->getResource<resource::IniFile>("devUserConfigFile"_id);
	if (debugConfigFile->getOrCreate("overlay", "enableEngineOverlay", false)) {
		LOG_TRACE(Log::Engine, "Register Engine Overlays");
		_overlaySystem->addOverlay<MainOverlay>("Main", true, *_overlaySystem);
		_overlaySystem->addOverlay<EngineCoreOverlay>("Engine", false);
		_overlaySystem->addOverlay<RenderingOverlay>("Rendering", false, *_renderSystem, *_ecs);
		_overlaySystem->addOverlay<ResourcesOverlay>("Resources", false, *_resourceSystem);
		_overlaySystem->addOverlay<FsmOverlay>("Fsm", false, *_fsmExternalController, *_fsmInfo);
		_overlaySystem->addOverlay<InputOverlay>("Input", false, *_inputSystem, *_resourceSystem);
		_overlaySystem->addOverlay<ExampleOverlay>("ImGui Demo", false);
	}
}

void Engine::_unregisterOverlays() {
	auto debugConfigFile = _resourceSystem->getResource<resource::IniFile>("devUserConfigFile"_id);
	if (debugConfigFile->getOrCreate("overlay", "enableEngineOverlay", false)) {
		LOG_TRACE(Log::Engine, "Unregister Engine Overlays");
		_overlaySystem->removeOverlay("ImGui Demo");
		_overlaySystem->removeOverlay("Fsm");
		_overlaySystem->removeOverlay("Input");
		_overlaySystem->removeOverlay("Resources");
		_overlaySystem->removeOverlay("Rendering");
		_overlaySystem->removeOverlay("Engine");
		_overlaySystem->removeOverlay("Main");
	}
}

void Engine::_throwIfAnyResourceIsStillLoaded() {
	LOG_TRACE(Log::Engine, "Check if any resource is still loaded");
	_resourceSystem->forEachManager([](auto& manager) {
		manager.forEachResource([](const auto& resource) {
			TACTICS_EXCEPTION("Resource [{}]:[{}] of type [{}] was not unloaded.",
							  resource.name,
							  resource.id,
							  toString(resource.type));
		});
	});
}

void Engine::_throwIfAnyImportantLogHappened() {
	if (Log::hasBeenLoggedOverLevel(LogLevel::Warning)) {
		TACTICS_EXCEPTION("Do not ignore logs over warning level.\nRecap:\n{}", Log::getLogCountRecapMessage());
	}
}

void Engine::_initializeSDL() {
	LOG_TRACE(Log::Engine, "SDL Initialization Started");
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
		TACTICS_EXCEPTION("SDL could not initialize! SDL_Error: {}\n", SDL_GetError());
	}
	LOG_TRACE(Log::Engine, "SDL Initialization Ended");
}

void Engine::_setupFsm(Application& application) {
	LOG_TRACE(Log::Engine, "Building Game FSM");
	auto builder = FsmBuilder();
	auto fsmStartingStateName = application.initialize(*_serviceLocator, builder);
	if (fsmStartingStateName.isEmpty()) {
		TACTICS_EXCEPTION(
			"Application did not return a valid name for the starting state for the FSM. The name is empty");
	}

	_fsmExternalController = std::make_unique<DefaultFsmExternalController>();
	std::tie(_fsm, _fsmInfo) = builder.build(fsmStartingStateName, _fsmExternalController.get());
	_eventsSystem->registerEventsListener(_fsmExternalController.get());
}

void Engine::_setupServiceLocator() {
	_serviceLocator = std::make_unique<ServiceLocator>();
	_serviceLocator->addService(_inputSystem.get());
	_serviceLocator->addService(_resourceSystem.get());
	_serviceLocator->addService(_overlaySystem.get());
	_serviceLocator->addService(_renderSystem.get());
	_serviceLocator->addService(_particleSystem.get());
	_serviceLocator->addService(_eventsSystem.get());
	_serviceLocator->addService(_ecs.get());
	_serviceLocator->addService(_sceneSystem.get());
	_serviceLocator->addService(_fileSystem.get());
	_serviceLocator->addService(_uiSystem.get());
	_serviceLocator->addService(_physicsSystem.get());
	_serviceLocator->addService(_dataSetSystem.get());
}

void Engine::_updateCommonComponentSystems() {
	using namespace component;

	auto& registry = _ecs->sceneRegistry();
	_physicsSystem->update(EngineTime::fixedDeltaTime<float>(), registry);
	SpriteAnimationSystem::update(registry);
	SpriteSystem::update(registry);
	CameraSystem::updateCameraAspectRatios(*_renderSystem, registry);
	CameraSystem::updateCameraMatrices(registry);
	_particleSystem->update(registry);
	BillboardSystem::update(registry);
	TransformSystem::updateTransformMatrices(registry);
	DebugDrawingSystem::update(registry);
}

} // namespace tactics
