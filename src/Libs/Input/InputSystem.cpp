#include "InputSystem.h"

#include "Click/Backend/ClickSDLBackend.h"
#include "InputSerialization.h"

#include <Libs/Resource/IniFile/IniFile.h>
#include <Libs/Resource/Input/InputMap.h>
#include <Libs/Resource/ResourceManager.h>
#include <Libs/Resource/ResourceProvider.h>
#include <Libs/Utility/HashId.h>
#include <Libs/Utility/Log/Log.h>
#include <Libs/Utility/Time/EngineTime.h>

namespace tactics {

void InputSystem::_onDeviceChanged(click::DeviceId id,
								   click::DeviceType type,
								   click::DeviceEvent event,
								   void* userData) {
	switch (event) {
	case click::DeviceEvent::Added: {
		LOG_TRACE(Log::Input, "Device added. Id: {}, Type: {}", id, type);
		break;
	}
	case click::DeviceEvent::Removed: {
		LOG_TRACE(Log::Input, "Device removed. Id: {}, Type: {}", id, type);
		break;
	}
	}

	auto inputSystem = static_cast<InputSystem*>(userData);
	inputSystem->_updateDeviceAssignment();
}

InputSystem::InputSystem(std::shared_ptr<resource::IniFile> configFile,
						 resource::ResourceProvider& resourceProvider,
						 const glm::vec2& screenSize)
	: _resourceProvider(resourceProvider) {
	LOG_TRACE(Log::Input, "Initialize click library");
	click::initialize(configFile->get("Engine", "players", 1), screenSize.x, screenSize.y);
	click::setDeviceChangedCallback(&InputSystem::_onDeviceChanged, this);
	click::initSdlBackend();
}

InputSystem::~InputSystem() {
	LOG_TRACE(Log::Input, "Shutting down click library");
	click::shutdown();
}

unsigned int InputSystem::getNumPlayers() const {
	return click::players();
}

const click::Player& InputSystem::getPlayer(click::PlayerId playerId) const {
	return click::player(playerId);
}

void InputSystem::processEvents(SDL_Event& event) {
	click::processSdlEvents(event);
}

void InputSystem::changeScreenSize(const glm::vec2& screenSize) {
	click::changeScreenSize(screenSize.x, screenSize.y);
}

void InputSystem::update() {
	click::updateSdlBackend();
	click::update(EngineTime::fixedDeltaTime<float>());

	auto& manager = _resourceProvider.getManager(resource::ResourceType::InputAction);
	manager.forEachTResource<resource::InputAction>([&](resource::InputAction& inputAction) {
		for (auto playerIndex = 0u; playerIndex < click::players(); ++playerIndex) {
			auto& actionState = getActionState(inputAction.actionId, 0);
			inputAction.states[playerIndex] = actionState;
		}
	});
}

void InputSystem::assignInputMap(std::shared_ptr<resource::InputMap> inputMap, click::PlayerId playerId) {
	auto mapId = click::addInputMap(playerId);
	for (const auto& binding : inputMap->bindings) {
		click::bind(mapId, binding.action->actionId, binding.gesture, binding.triggers, binding.modifiers);
	}
}

void InputSystem::assignDevice(click::DeviceType deviceType, unsigned int deviceIndex, click::PlayerId playerId) {
	auto itr = _playerDevices.find(playerId);
	if (itr == _playerDevices.end()) {
		_playerDevices[playerId] = std::make_tuple(deviceType, deviceIndex);
	} else {
		itr->second = std::make_tuple(deviceType, deviceIndex);
	}

	_updateDeviceAssignment();
}

void InputSystem::assignKeyboard(click::PlayerId playerId) {
	assignDevice(click::DeviceType::Keyboard, 0, playerId);
}

void InputSystem::assignMouse(click::PlayerId playerId) {
	assignDevice(click::DeviceType::Mouse, 0, playerId);
}

void InputSystem::assignGamepad(click::PlayerId playerId, unsigned int deviceIndex) {
	assignDevice(click::DeviceType::Gamepad, deviceIndex, playerId);
}

void InputSystem::_updateDeviceAssignment() {
	for (const auto& playerDevice : _playerDevices) {
		auto deviceType = std::get<0>(playerDevice.second);
		auto deviceIndex = std::get<1>(playerDevice.second);
		if (hasDevice(deviceType, deviceIndex)) {
			auto deviceId = getDeviceId(deviceType, deviceIndex);
			click::holdDevice(playerDevice.first, deviceId);
		}
	}
}

bool InputSystem::hasDevice(click::DeviceType deviceType, unsigned int deviceIndex) const {
	switch (deviceType) {
	case click::DeviceType::Keyboard: return click::hasKeyboard(deviceIndex);
	case click::DeviceType::Mouse	: return click::hasMouse(deviceIndex);
	case click::DeviceType::Gamepad : return click::hasGamepad(deviceIndex);
	}

	LOG_ERROR(Log::Input,
			  "Wrong device type provided while trying to check if the device exists. Type: {}, Index: {}",
			  deviceType,
			  deviceIndex);
	return false;
}

click::DeviceId InputSystem::getDeviceId(click::DeviceType deviceType, unsigned int deviceIndex) const {
	switch (deviceType) {
	case click::DeviceType::Keyboard: return click::keyboard(deviceIndex);
	case click::DeviceType::Mouse	: return click::mouse(deviceIndex);
	case click::DeviceType::Gamepad : return click::gamepad(deviceIndex);
	}

	LOG_ERROR(Log::Input,
			  "Wrong device type provided while trying to retrieve the DeviceId. Type: {}, Index: {}",
			  deviceType,
			  deviceIndex);
	return 0;
}

const click::ActionState& InputSystem::getActionState(click::ActionId actionId, click::PlayerId playerId) {
	return click::actionState(actionId, playerId);
}

} // namespace tactics
