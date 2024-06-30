#pragma once

#include <Libs/Resource/IniFile/IniFile.h>

#include <glm/glm.hpp>
#include <memory>
#include <vector>

struct SDL_Window;

namespace tactics {
namespace resource {
class IniFile;
}

class DebugMessageHandler;
class RenderQueue;

class RenderSystem {
public:
	RenderSystem(std::shared_ptr<resource::IniFile> configFile);
	~RenderSystem();

	RenderQueue& createRenderQueue();

	void render();

	glm::vec2 getWindowSize() const;

private:
	void _createWindow();
	void _initializeGlContext();

	void _initializeImGui();
	void _shutdownImGui();

	void _setupVSync();

	void _setupGlAttributes();
	void _setupDoubleBuffer();
	void _setupOglVersion();
	void _setupOglContextProfile();

	template<typename TValue> TValue _getConfigValue(std::string_view fieldName, const TValue& defaultValue) {
		return _configFile->getOrCreate("Graphics", fieldName, defaultValue);
	}

	SDL_Window* _window{};
	void* _oglContext{};
	std::unique_ptr<DebugMessageHandler> _debugMessageHandler;
	std::vector<std::unique_ptr<RenderQueue>> _renderQueues;
	std::shared_ptr<resource::IniFile> _configFile;
};
} // namespace tactics
