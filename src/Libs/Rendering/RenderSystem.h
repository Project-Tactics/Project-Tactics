#pragma once

#include "Viewport.h"

#include <Libs/Event/EventsListener.h>
#include <Libs/Resource/IniFile/IniFile.h>

#include <glm/glm.hpp>
#include <memory>
#include <vector>

struct SDL_Window;
union SDL_Event;

namespace tactics {
namespace resource {
class IniFile;
}

class DebugMessageHandler;
class RenderQueue;
struct Viewport;

class RenderSystem : public EventsListener {
public:
	RenderSystem(std::shared_ptr<resource::IniFile> configFile);
	~RenderSystem();

	RenderQueue& createRenderQueue();
	void destroyRenderQueues();

	void render();

	const glm::u32vec2& getWindowSize() const;

	void setViewport(const glm::vec2& position, const glm::vec2& size, glm::vec4 clearColor);
	const Viewport& getViewport() const;

	bool onEvent(const SDL_Event& event) override;

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
	Viewport _viewport;
	glm::u32vec2 _windowSize;
};
} // namespace tactics
