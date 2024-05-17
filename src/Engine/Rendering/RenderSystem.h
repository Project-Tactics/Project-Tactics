#pragma once

#include <memory>
#include <vector>

struct SDL_Window;

namespace tactics {

class DebugMessageHandler;
class RenderQueue;

class RenderSystem {
public:
	RenderSystem();
	~RenderSystem();

	RenderQueue& createRenderQueue();

	void render();

private:
	void _defineGlAttributes(bool useDebugMessages);
	void _createWindow();
	void _initializeGlContext();

	SDL_Window* _window;
	void* _oglContext;
	std::unique_ptr<DebugMessageHandler> _debugMessageHandler;
	std::vector<std::unique_ptr<RenderQueue>> _renderQueues;
};
}
