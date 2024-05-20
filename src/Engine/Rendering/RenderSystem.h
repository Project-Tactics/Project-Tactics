#pragma once

#include <Libs/Resource/IniFile/IniFile.h>

#include <memory>
#include <vector>

struct SDL_Window;

namespace tactics {

class Camera;
class DebugMessageHandler;
class RenderQueue;
class Viewport;

class RenderSystem {
public:
	RenderSystem(IniFile* configFile);
	~RenderSystem();

	RenderQueue& createRenderQueue();

	Camera& getCamera();
	Viewport& getViewport();

	void render();

private:
	Camera& _createCamera();
	Viewport& _createViewport(Camera& camera);

	void _defineGlAttributes(bool useDebugMessages);
	void _createWindow();
	void _initializeGlContext();

	void _initializeImGui();
	void _shutdownImGui();

	SDL_Window* _window{};
	void* _oglContext{};
	std::unique_ptr<DebugMessageHandler> _debugMessageHandler;
	std::vector<std::unique_ptr<RenderQueue>> _renderQueues;
	std::unique_ptr<Viewport> _viewport;
	std::unique_ptr<Camera> _camera;
	IniFile* _configFile;
};
}
