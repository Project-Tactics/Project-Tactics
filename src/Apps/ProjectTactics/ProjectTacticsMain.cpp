#include <iostream>

#include <SDL.h>
#include <glad/gl.h>

int main(int argc, char** argv) {

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return 0;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	auto* window = SDL_CreateWindow("Project Tactics", 500, 500, 640, 480, SDL_WINDOW_OPENGL);
	if (window == nullptr) {
		printf("Failed to open window: %s\n", SDL_GetError());
		return 0;
	}

	auto openGlContext = SDL_GL_CreateContext(window);

	int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);

	SDL_Event event;
	while (1) {
		SDL_PollEvent(&event);

		switch (event.type) {
		case SDL_QUIT:
			return 0;
		}

		glViewport(0, 0, 600, 400);
		auto error = glGetError();
		glClearColor(1.f, 0.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(openGlContext);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
