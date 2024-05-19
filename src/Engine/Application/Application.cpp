#include "Application.h"

#include "States/MapState.h"
#include "States/StartState.h"

#include <Engine/Rendering/RenderSystem.h>
#include <Engine/Resource/ResourceSystemInitializer.h>

#include <Libs/Events/EventsSystem.h>
#include <Libs/Fsm/FsmBuilder.h>
#include <Libs/Overlay/OverlaySystem.h>
#include <Libs/Resource/ResourceSystem.h>
#include <Libs/Resource/IniFile/IniFile.h>
#include <Libs/Utilities/Exception.h>

#include <imgui/imgui.h>
#include <SDL.h>
#include <exception>
#include <format>

namespace tactics {

Application::Application() {
}

Application::~Application() {
}

void Application::run() {
	try {
		_initialize();
		_internalRun();
		_shutdown();
	}
	catch (Exception& exception) {
		printf("%s", exception.what());
	}
	catch (std::exception& exception) {
		printf("%s", exception.what());
	}
}

void Application::_initialize() {
	_initializeSDL();
	_initializeResourceSystem();
	_initializeRenderSystem();
	_loadResources();
	_initializeEventsSystem();
	_initializeOverlaySystem();
	_initializeFsm();
}

void Application::_internalRun() {
	while (!_fsm->hasReachedExitState()) {
		auto eventResult = _eventsSystem->update();
		if (eventResult == EventResult::QuitGame) {
			return;
		}

		_fsm->update();

		_renderSystem->render();
	}
}

void Application::_shutdown() {
	_eventsSystem->unregisterEventsListener(_fsm.get());
	SDL_Quit();
}

void Application::_initializeSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw std::exception(std::format("SDL could not initialize! SDL_Error: {}\n", SDL_GetError()).c_str());
	}
}

void Application::_initializeRenderSystem() {
	_renderSystem = std::make_unique<RenderSystem>();

	// TODO(Gerark) This is absolutely something that should move elsewhere and probably configure it through lua
	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;

	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 0.80f);
	style->Colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.25f, 0.28f, 0.25f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.26f, 0.28f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);

	std::filesystem::path fontPath = _resourceSystem->getResourcePathHelper().makeAbsolutePath("debug\\ruda-bold.ttf");
	if (std::filesystem::exists(fontPath)) {
		auto fontPathStr = fontPath.string();
		auto& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF(fontPathStr.c_str(), 10);
		io.Fonts->AddFontFromFileTTF(fontPathStr.c_str(), 12);
		auto defaultFont = io.Fonts->AddFontFromFileTTF(fontPathStr.c_str(), 14);
		io.FontDefault = defaultFont;
		io.Fonts->AddFontFromFileTTF(fontPathStr.c_str(), 18);
		io.Fonts->AddFontFromFileTTF(fontPathStr.c_str(), 24);
		io.Fonts->AddFontFromFileTTF(fontPathStr.c_str(), 32);
	}
}

void Application::_initializeEventsSystem() {
	_eventsSystem = std::make_unique<EventsSystem>();
}

void Application::_initializeFsm() {
	auto builder = FsmBuilder();

	builder
		.state<StartState>("Start", *_resourceSystem, *_renderSystem, *_overlaySystem)
		.on("proceed").jumpTo("Map")

		.state<MapState>("Map", *_renderSystem, *_overlaySystem)
		.on("exit").exitFsm();

	_fsm = builder.build("Start");

	_eventsSystem->registerEventsListener(_fsm.get());
}

void Application::_initializeOverlaySystem() {
	auto iniFile = _resourceSystem->getResource<IniFile>("configFile");
	_overlaySystem = std::make_unique<OverlaySystem>(std::move(iniFile));
	_overlaySystem->setEnabled(true);
}

void Application::_initializeResourceSystem() {
	_resourceSystem = std::make_unique<ResourceSystem>("data");
	ResourceSystemInitializer::initialize(*_resourceSystem);
}

void Application::_loadResources() {
	_resourceSystem->loadResourcePackDefinition("engine_res.lua");
	_resourceSystem->loadResourcePack("initialization");
}

}
