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
#include <Libs/Utility/Service/ServiceLocator.h>
#include <Libs/Utility/Exception.h>
#include <Libs/Utility/Math.h>

#include <imgui/imgui.h>
#include <SDL.h>
#include <fmt/format.h>
#include <source_location>

namespace tactics {

void _printCallstack(const std::stacktrace& callstack) {
	auto currentStackTrace = std::stacktrace::current();
	std::filesystem::path rootPath((*currentStackTrace.begin()).source_file());
	// TODO(Gerark) Very hacky to retrieve the src base folder
	rootPath = rootPath.parent_path().parent_path().parent_path().parent_path();
	for (auto&& entry : callstack) {
		std::filesystem::path entryPath(entry.source_file());
		std::filesystem::path relativePath = entryPath.lexically_relative(rootPath);
		auto stackEntryStr = fmt::format("---\n.\\{}:{}\n{}\n", relativePath.string(), entry.source_line(), entry.description());
		printf("%s", stackEntryStr.c_str());
	}
}

void Engine::_run(Application& application) {
	try {
		auto engine = Engine();
		engine._initialize(application);
		engine._internalRun();
		engine._shutdown();
	}
	catch (Exception& exception) {
		// TODO(Gerark) Add a logger
		printf("%s\nCallstack:\n", exception.what());
		_printCallstack(exception.stackTrace());
	}
	catch (std::exception& exception) {
		printf("%s", exception.what());
	}
}

void Engine::_initialize(Application& application) {
	_initializeSDL();

	auto pathHelper = std::make_unique<PathHelper>("data");
	auto fileLoader = std::make_unique<DefaultFileLoader>(*pathHelper.get());
	_fileSystem = std::make_unique<FileSystem>(std::move(fileLoader), std::move(pathHelper));

	_ecs = std::make_unique<EntityComponentSystem>();

	_resourceSystem = ResourceSystemInitializer::initialize(*_fileSystem, *_ecs);

	auto devUserConfigFile = _resourceSystem->getResource<resource::IniFile>("devUserConfigFile");
	auto imguiSettings = _resourceSystem->getResource<resource::IniFile>("imguiSettings");
	_overlaySystem = std::make_unique<OverlaySystem>(devUserConfigFile, *imguiSettings, *_fileSystem);
	_overlaySystem->setEnabled(true);
	CustomOverlayColors::initialize(*imguiSettings);

	auto configFile = _resourceSystem->getResource<resource::IniFile>("configFile");
	_renderSystem = std::make_unique<RenderSystem>(configFile);
	_resourceSystem->loadPack("builtinMeshes");
	_resourceSystem->createManualPack("_internalCustomPack");
	_resourceSystem->loadExternalResource("_internalCustomPack", resource::Texture::createNullTexture());

	_eventsSystem = std::make_unique<EventsSystem>();

	_sceneSystem = std::make_unique<SceneSystem>(*_ecs, *_resourceSystem);

	_setupServiceLocator();
	_setupFsm(application);

	if (devUserConfigFile->getOrCreate("overlay", "enableEngineOverlay", false)) {
		_overlaySystem->addOverlay<MainOverlay>("Main", true, *_overlaySystem);
		_overlaySystem->addOverlay<RenderingOverlay>("Rendering", false, *_renderSystem, *_ecs);
		_overlaySystem->addOverlay<ResourcesOverlay>("Resources", false, *_resourceSystem);
		_overlaySystem->addOverlay<FsmOverlay>("Fsm", false, *_fsmExternalController, *_fsmInfo);
		_overlaySystem->addOverlay<ExampleOverlay>("ImGui Demo", false);
	}
}

void Engine::_internalRun() {
	while (!_fsm->hasReachedExitState()) {
		auto eventResult = _eventsSystem->update();
		if (eventResult == EventResult::QuitGame) {
			return;
		}

		_fsm->update();
		_updateCommonComponentSystems();
		_renderSystem->render();
	}
}

void Engine::_shutdown() {
	auto debugConfigFile = _resourceSystem->getResource<resource::IniFile>("devUserConfigFile");
	if (debugConfigFile->getOrCreate("overlay", "enableEngineOverlay", false)) {
		_overlaySystem->removeOverlay("Main");
		_overlaySystem->removeOverlay("Rendering");
		_overlaySystem->removeOverlay("ImGui Demo");
	}
	debugConfigFile.reset();

	_eventsSystem->unregisterEventsListener(_fsm.get());
	_renderSystem.reset();
	_overlaySystem.reset();
	_resourceSystem->unloadPack("initialization");
	_resourceSystem->unloadPack("builtinMeshes");
	_resourceSystem->unloadPack("_internalCustomPack");
	_throwIfAnyResourceIsStillLoaded();
	SDL_Quit();
}

void Engine::_throwIfAnyResourceIsStillLoaded() {
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

void Engine::_initializeSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw TACTICS_EXCEPTION("SDL could not initialize! SDL_Error: {}\n", SDL_GetError());
	}
}

void Engine::_setupFsm(Application& application) {
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

	_ecs->ctx().emplace<ServiceLocator*>(_serviceLocator.get());
}

void Engine::_updateCommonComponentSystems() {
	using namespace component;

	CameraSystem::updateCameraMatrices(_ecs->view<Frustum, Transform, Camera>());
	CameraSystem::updateCameraAspectRatios(
		_ecs->view<Viewport, CurrentViewport>(),
		_ecs->view<Frustum, CurrentCamera>());
	TransformSystem::updateTransformMatrices(_ecs->view<Transform>());
}

}
