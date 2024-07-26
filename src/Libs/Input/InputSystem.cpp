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
		for (click::PlayerId playerIndex = 0u; playerIndex < click::players(); ++playerIndex) {
			auto& actionState = getActionState(inputAction.actionId, playerIndex);
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
	_playerDevices[playerId].push_back({deviceType, deviceIndex});
	_updateDeviceAssignment();
}

bool InputSystem::hasDeviceAssigned(click::PlayerId playerId, click::DeviceType deviceType) const {
	auto it = _playerDevices.find(playerId);
	if (it == _playerDevices.end()) {
		return false;
	}

	return std::count_if(it->second.begin(), it->second.end(), [&](const auto& device) {
			   return std::get<0>(device) == deviceType;
		   }) > 0;
}

void InputSystem::_updateDeviceAssignment() {
	for (const auto& [playerId, devices] : _playerDevices) {
		for (auto& [deviceType, deviceIndex] : devices) {
			if (!hasDevice(deviceType, deviceIndex)) {
				continue;
			}

			auto deviceId = getDeviceId(deviceType, deviceIndex);
			click::holdDevice(playerId, deviceId);
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

const click::DeviceData& InputSystem::getDevice(click::DeviceId deviceId) const {
	return click::device(deviceId);
}

const click::ActionState& InputSystem::getActionState(click::ActionId actionId, click::PlayerId playerId) const {
	return click::actionState(actionId, playerId);
}

const click::ActionValue& InputSystem::getInputCodeValue(click::InputCode inputCode, click::PlayerId playerId) const {
	return click::inputValue(inputCode, playerId);
}

bool InputSystem::isInputCodeTriggered(click::InputCode inputCode, click::PlayerId playerId) const {
	return click::_magnitudeSquared(click::inputValue(inputCode, playerId)) != 0;
}

bool InputSystem::checkAction(const char* inputActionName, click::PlayerId playerId) const {
	auto actionId = HashId(inputActionName);
	return getActionState(actionId, playerId).state == click::InputState::Triggered;
}

} // namespace tactics
