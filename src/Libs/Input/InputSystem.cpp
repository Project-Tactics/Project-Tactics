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

InputSystem::InputSystem(std::shared_ptr<resource::IniFile> configFile,
						 resource::ResourceProvider& resourceProvider,
						 const glm::vec2& screenSize)
	: _resourceProvider(resourceProvider) {
	LOG_TRACE(Log::Input, "Initialize click library");
	click::initialize(configFile->get("Engine", "players", 1), screenSize.x, screenSize.y);
	click::initSdlBackend();
}

InputSystem::~InputSystem() {
	LOG_TRACE(Log::Input, "Shutting down click library");
	click::shutdown();
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

void InputSystem::assignDevice(click::DeviceId deviceId, click::PlayerId playerId) {
	click::holdDevice(playerId, deviceId);
}

click::DeviceId InputSystem::getDeviceId(click::DeviceType deviceType, unsigned int deviceIndex) {
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
