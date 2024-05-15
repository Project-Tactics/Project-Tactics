#include "RenderSystem.h"

#include "ShaderLoader.h"

#include <glad/gl.h>
#include <SDL.h>
#include <iostream>
#include <format>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_sdl2.h>

namespace tactics {
RenderSystem::RenderSystem() {
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	_window = SDL_CreateWindow("Project Tactics", 100, 100, 1280, 720, SDL_WINDOW_OPENGL);
	if (_window == nullptr) {
		throw std::exception(std::format("Failed to open window: %s\n", SDL_GetError()).c_str());
	}

	_oglContext = SDL_GL_CreateContext(_window);
	SDL_GL_MakeCurrent(_window, _oglContext);
	SDL_GL_SetSwapInterval(1);

	int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
	if (version == 0) {
		throw std::exception(std::format("Failed to initialize OpenGL context\n").c_str());
	}

	printf(std::format("Loaded OpenGL {}.{}\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version)).c_str());

	glViewport(0, 0, 1280, 720);
	glClearColor(0.f, 0.f, 0.f, 1.f);

	_loadShaders();
	_createQuad();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	ImGui_ImplSDL2_InitForOpenGL(_window, _oglContext);
	ImGui_ImplOpenGL3_Init();
}

RenderSystem::~RenderSystem() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(_oglContext);
	SDL_DestroyWindow(_window);
}

void RenderSystem::beginDraw() {
	glClear(GL_COLOR_BUFFER_BIT);
}

void RenderSystem::endDraw() {
	SDL_GL_SwapWindow(_window);
}

void RenderSystem::drawSomething() {
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void RenderSystem::_createQuad() {
	unsigned int buffer;
	glGenBuffers(1, &buffer);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	float positions[6] = {
		-0.5f, -0.5f,
		0.0f, 0.5f,
		0.5f, -0.5f
	};

	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	_checkGlErrors("After Create Quad");
}

void RenderSystem::_loadShaders() {
	auto programId = ShaderLoader::loadProgram(
		R"(
			#version 330 core
			layout(location = 0) in vec4 position;
			void main()
			{
				gl_Position = position;
			}
		)",
		R"(
			#version 330 core
			layout(location = 0) out vec4 color;
			void main()
			{
				color = vec4(1.0, 0.5, 0.0, 1.0);
			}			
		)"
	);
	glUseProgram(programId);
}

void RenderSystem::_checkGlErrors(const char* context) {
	auto errorCode = glGetError();
	if (errorCode != GL_NO_ERROR) {
		std::string error;

		switch (errorCode) {
		case GL_INVALID_ENUM:
			error = "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
			break;
		case GL_INVALID_VALUE:
			error = "GL_INVALID_VALUE: A numeric argument is out of range.";
			break;
		case GL_INVALID_OPERATION:
			error = "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			error = "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
			break;
		case GL_OUT_OF_MEMORY:
			error = "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
			break;
		case GL_STACK_UNDERFLOW:
			error = "GL_STACK_UNDERFLOW: An attempt has been made to perform an operation that would cause an internal stack to underflow.";
			break;
		case GL_STACK_OVERFLOW:
			error = "GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would cause an internal stack to overflow.";
			break;
		default:
			error = "UNKNOWN ERROR: An unknown OpenGL error occurred.";
			break;
		}

		throw std::exception(std::format("OpenGL error in {}: {} - Code: {}", context, error, errorCode).c_str());
	}
}

void RenderSystem::beginDrawOverlay() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void RenderSystem::endDrawOverlay() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

}
