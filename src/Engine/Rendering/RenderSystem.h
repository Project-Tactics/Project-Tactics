#pragma once

#include <memory>
#include <vector>

struct SDL_Window;

namespace tactics {

class Renderer;

class RenderSystem {
public:
	RenderSystem();
	~RenderSystem();

	Renderer& createRenderer();

	void render();

private:
	void _defineGlAttributes();
	void _createWindow();
	void _initializeGlContext();
	void _checkGlErrors(const char* context);
	void _createQuad();
	void _loadShaders();

	SDL_Window* _window;
	void* _oglContext;
	std::vector<std::unique_ptr<Renderer>> _renderers;
};
}
