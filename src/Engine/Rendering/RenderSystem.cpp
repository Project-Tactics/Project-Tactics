#include "RenderSystem.h"

#include "Camera.h"
#include "Viewport.h"
#include "DebugMessageHandler.h"
#include "RenderQueue.h"
#include "RenderSteps/RenderStep.h"

#include <Libs/Utilities/Exception.h>

#include <glad/gl.h>
#include <SDL.h>
#include <iostream>
#include <format>
#include <filesystem>

#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_sdl2.h>

namespace tactics {

// TODO(Gerark) This is a very simple way of dealing with OGL errors and requires an improvement
// We can use all the parameters and we might have a dedicated class for it. Also it's currently stateless, which is not bad per se,
// but we could probably pass a valid *userParam and leverage that by using a proper log manager
void onGlErrorMessage(GLenum /*source*/, GLenum /*type*/, GLuint /*id*/, GLenum /*severity*/, GLsizei /*length*/, const GLchar* message, const void* /*userParam*/) {
	printf("%s\n", message);
}

RenderSystem::RenderSystem(IniFile& configFile): _configFile(configFile) {
	static const bool useDebugMessages = true;
	_defineGlAttributes(useDebugMessages);
	_createWindow();
	_initializeGlContext();

	auto& camera = _createCamera();
	_createViewport(camera);

	_initializeImGui();

	if (useDebugMessages) {
		_debugMessageHandler = std::make_unique<DebugMessageHandler>();
	}

	glEnable(GL_DEPTH_TEST);
}

RenderSystem::~RenderSystem() {
	_shutdownImGui();
	SDL_GL_DeleteContext(_oglContext);
	SDL_DestroyWindow(_window);
}

RenderQueue& RenderSystem::createRenderQueue() {
	auto renderQueue = std::make_unique<RenderQueue>();
	_renderQueues.push_back(std::move(renderQueue));
	return *_renderQueues.back().get();
}

void RenderSystem::_defineGlAttributes(bool useDebugMessages) {
	// TODO(Gerark) all these configurations could be moved to lua/config

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	int majorVersion = 3;
	int minorVersion = 2;

	// We're using OpenGL 4.3 to get access to the glDebugMessageCallback and get better error message. We might decrease the version if this is
	// the only functionality we need and we're releasing a final version which doesn't require the log part
	if (useDebugMessages) {
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
		majorVersion = 4;
		minorVersion = 3;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, majorVersion);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minorVersion);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
}

void RenderSystem::_createWindow() {
	int width = _configFile.getOrCreate("window", "width", 1280);
	int height = _configFile.getOrCreate("window", "height", 720);
	_window = SDL_CreateWindow("Project Tactics", 100, 100, width, height, SDL_WINDOW_OPENGL);
	if (_window == nullptr) {
		throw Exception("Failed to open window: %s\n", SDL_GetError());
	}
}

void RenderSystem::_initializeGlContext() {
	_oglContext = SDL_GL_CreateContext(_window);
	SDL_GL_MakeCurrent(_window, _oglContext);
	SDL_GL_SetSwapInterval(1);

	int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
	if (version == 0) {
		throw Exception("Failed to initialize OpenGL context\n");
	}

	// TODO(Gerark) Move this printf once we have a proper Log System ( don't forget of removing the useless headers after that )
	printf(std::format("Loaded OpenGL {}.{}\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version)).c_str());
}

void RenderSystem::render() {
	RenderStepInfo renderInfo{*_camera, *_viewport};

	_camera->update();
	glViewport(_viewport->getTopLeft().x, _viewport->getTopLeft().y, _viewport->getSize().x, _viewport->getSize().y);
	for (auto& renderQueue : _renderQueues) {
		renderQueue->execute(renderInfo);
	}
	SDL_GL_SwapWindow(_window);
}

Camera& RenderSystem::_createCamera() {
	_camera = std::make_unique<Camera>();
	return *_camera;
}

Viewport& RenderSystem::_createViewport(Camera& camera) {
	_viewport = std::make_unique<Viewport>(camera);
	int width, height;
	SDL_GetWindowSize(_window, &width, &height);
	_viewport->setDimensions(glm::vec2{0, 0}, glm::vec2{width, height});
	return *_viewport;
}

Camera& RenderSystem::getCamera() {
	if (!_camera) {
		throw Exception("Can't get camera. The camera hasn't been created yet.");
	}
	return *_camera;
}

void RenderSystem::_initializeImGui() {
	// TODO(Gerark) Creating the context of ImGui in the RenderSystem doesn't sound right.
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	ImGui_ImplSDL2_InitForOpenGL(_window, _oglContext);
	ImGui_ImplOpenGL3_Init();
}

void RenderSystem::_shutdownImGui() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

Viewport& RenderSystem::getViewport() {
	return *_viewport;
}

}
