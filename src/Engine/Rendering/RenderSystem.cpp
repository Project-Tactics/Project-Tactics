#include "RenderSystem.h"

#include <glad/gl.h>
#include <SDL.h>

namespace tactics {
RenderSystem::RenderSystem(SDL_Window* window)
	: _window(window) {
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	_oglContext = SDL_GL_CreateContext(_window);

	/*int version = */gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
}

RenderSystem::~RenderSystem() {
	SDL_GL_DeleteContext(_oglContext);
}

void RenderSystem::beginDraw() {
	glViewport(0, 0, 600, 400);
	glClearColor(1.f, 0.f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void RenderSystem::endDraw() {
	SDL_GL_SwapWindow(_window);
}
}
