#pragma once

#include <memory>
#include <vector>

struct SDL_Window;

namespace tactics {

class DebugMessageHandler;
class RenderQueue;
class Camera;

class RenderSystem {
public:
	RenderSystem();
	~RenderSystem();

	RenderQueue& createRenderQueue();
	Camera& createCamera();

	Camera& getCamera();

	void render();

private:
	void _defineGlAttributes(bool useDebugMessages);
	void _createWindow();
	void _initializeGlContext();

	void _initializeImGui();
	void _shutdownImGui();

	SDL_Window* _window{};
	void* _oglContext{};
	std::unique_ptr<DebugMessageHandler> _debugMessageHandler;
	std::vector<std::unique_ptr<RenderQueue>> _renderQueues;
	std::unique_ptr<Camera> _camera;
};
}
