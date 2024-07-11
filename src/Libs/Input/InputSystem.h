#pragma once

#include "Click/Click.h"

#include <SDL.h>
#include <memory>

namespace tactics {
namespace resource {
class IniFile;
class InputMap;
class InputAction;
class ResourceProvider;
} // namespace resource

class InputSystem {
public:
	InputSystem(std::shared_ptr<resource::IniFile> configFile, resource::ResourceProvider& resourceProvider);

	void processEvents(SDL_Event& event);
	void update();

	void assignInputMap(std::shared_ptr<resource::InputMap> inputMap, click::PlayerId playerId);
	void assignDevice(click::DeviceId deviceId, click::PlayerId playerId);
	click::DeviceId getDeviceId(click::DeviceType deviceType, unsigned int deviceIndex);
	const click::ActionState& getActionState(click::ActionId actionId, click::PlayerId playerId);

private:
	resource::ResourceProvider& _resourceProvider;
};

} // namespace tactics
