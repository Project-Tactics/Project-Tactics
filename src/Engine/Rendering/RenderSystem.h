#pragma once

#include <memory>

struct SDL_Window;

namespace tactics {

class Renderer;

class RenderSystem {
public:
	RenderSystem();
	~RenderSystem();

	void beginDraw();
	void drawSomething();
	void endDraw();

	void beginDrawOverlay();
	void endDrawOverlay();

private:
	void _checkGlErrors(const char* context);
	void _createQuad();
	void _loadShaders();

	SDL_Window* _window;
	void* _oglContext;
	std::unique_ptr<Renderer> _renderer;
};
}
