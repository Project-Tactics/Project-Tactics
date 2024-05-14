#pragma once

#include <memory>

struct SDL_Window;

namespace tactics::engine {

class RenderSystem;

class Application {
public:
	Application();
	~Application();

	void run();

private:
	void _initialize();
	void _initializeSDL();
	void _createWindow();
	void _initializeRenderSystem();
	void _internalRun();
	void _shutdown();

	SDL_Window* _window;
	std::unique_ptr<RenderSystem> _renderSystem;
};

}
