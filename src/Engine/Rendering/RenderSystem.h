#pragma once

struct SDL_Window;

namespace tactics {
class RenderSystem {
public:
	RenderSystem(SDL_Window* window);
	~RenderSystem();

	void beginDraw();
	void endDraw();

private:
	SDL_Window* _window;
	void* _oglContext;
};
}
