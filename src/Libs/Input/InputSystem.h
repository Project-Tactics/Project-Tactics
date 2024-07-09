#pragma once

#include "Click/Click.h"

#include <SDL.h>
#include <memory>

namespace tactics {
namespace resource {
class IniFile;
}

class InputSystem {
public:
	InputSystem(std::shared_ptr<resource::IniFile> configFile);

	void processEvents(SDL_Event& event);
	void update();

private:
	click::ActionId _moveActionId;
	click::ActionId _mouseActionId;
	click::ActionId _spaceActionId;
};

} // namespace tactics
