#pragma once

#include "Click/Click.h"

#include <SDL.h>
#include <glm/vec2.hpp>
#include <memory>
#include <tuple>
#include <unordered_map>

namespace tactics {
namespace resource {
class IniFile;
class InputMap;
class InputAction;
class ResourceProvider;
} // namespace resource

class InputSystem {
public:
	InputSystem(std::shared_ptr<resource::IniFile> configFile,
				resource::ResourceProvider& resourceProvider,
				const glm::vec2& screenSize);
	~InputSystem();

	unsigned int getNumPlayers() const;
	const click::Player& getPlayer(click::PlayerId playerId) const;

	void changeScreenSize(const glm::vec2& screenSize);
	void processEvents(SDL_Event& event);
	void update();

	bool checkAction(const char* inputActionName, click::PlayerId playerId = 0) const;

	void assignInputMap(const char* inputMapName, click::PlayerId playerId = 0);
	void assignInputMap(std::shared_ptr<resource::InputMap> inputMap, click::PlayerId playerId = 0);
	void assignDevice(click::DeviceType deviceType, unsigned int deviceIndex, click::PlayerId playerId = 0);
	bool hasDeviceAssigned(click::PlayerId playerId, click::DeviceType deviceType) const;

	click::DeviceId getDeviceId(click::DeviceType deviceType, unsigned int deviceIndex) const;
	const click::DeviceData& getDevice(click::DeviceId deviceId) const;
	bool hasDevice(click::DeviceType deviceType, unsigned int deviceIndex) const;
	const click::ActionState& getActionState(click::ActionId actionId, click::PlayerId playerId = 0) const;
	const click::ActionValue& getInputCodeValue(click::InputCode inputCode, click::PlayerId playerId = 0) const;

private:
	void _updateDeviceAssignment();
	static void _onDeviceChanged(click::DeviceId id, click::DeviceType type, click::DeviceEvent event, void* userData);

	resource::ResourceProvider& _resourceProvider;
	std::unordered_map<click::PlayerId, std::vector<std::tuple<click::DeviceType, unsigned int>>> _playerDevices;
};

} // namespace tactics
