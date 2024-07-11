#include "Click.h"

#include "ClickModifiers.h"
#include "ClickTriggers.h"

#include <assert.h>

namespace click {

Context* ctx = nullptr;

auto _isValueZero(const ActionValue& value) {
	return value.vec3.x == 0.0f && value.vec3.y == 0.0f && value.vec3.z == 0.0f;
}

/*
 * Player
 */

void clearPlayer(PlayerId playerId) {
	auto& player = ctx->players[playerId];
	player.inputMaps.clear();
	player.heldDevices.clear();
}

unsigned int players() {
	return static_cast<unsigned int>(ctx->players.size());
}

/*
 * Device
 */

DeviceId _getNextAvailableDeviceId() {
	for (auto index = 0; index < ctx->devices.devices.size(); ++index) {
		if (ctx->devices.devices[index].type == DeviceType::None) {
			return static_cast<DeviceId>(index);
		}
	}
	assert(false);
	return 0;
}

DeviceId _createDevice(DeviceType type, void* customData) {
	auto nextId = _getNextAvailableDeviceId();
	ctx->devices.devices[nextId] = {type, nextId, customData};
	return nextId;
}

void installGamepad(void* customData) {
	auto id = _createDevice(DeviceType::Gamepad, customData);
	ctx->devices.gamepads.push_back(id);
}

void installKeyboard() {
	auto id = _createDevice(DeviceType::Keyboard, nullptr);
	ctx->devices.keyboards.push_back(id);
}

void installMouse() {
	auto id = _createDevice(DeviceType::Mouse, nullptr);
	ctx->devices.mice.push_back(id);
}

void uninstallMouse(DeviceId id) {
	auto& device = ctx->devices.devices[id];
	device.type = DeviceType::None;
	device.customData = nullptr;
	device.deviceId = 0;
	std::erase(ctx->devices.mice, id);
}

void uninstallKeyboard(DeviceId id) {
	auto& device = ctx->devices.devices[id];
	device.type = DeviceType::None;
	device.customData = nullptr;
	device.deviceId = 0;
	std::erase(ctx->devices.keyboards, id);
}

void uninstallGamepad(DeviceId id) {
	auto& device = ctx->devices.devices[id];
	device.type = DeviceType::None;
	device.customData = nullptr;
	device.deviceId = 0;
	std::erase(ctx->devices.gamepads, id);
}

DeviceId keyboard(unsigned int index) {
	return ctx->devices.keyboards[index];
}

DeviceId mouse(unsigned int index) {
	return ctx->devices.mice[index];
}

DeviceId gamepad(unsigned int index) {
	return ctx->devices.gamepads[index];
}

const DeviceIdList& gamepads() {
	return ctx->devices.gamepads;
}

const DeviceIdList& mice() {
	return ctx->devices.mice;
}

const DeviceIdList& keyboards() {
	return ctx->devices.keyboards;
}

const DeviceIdList& touches() {
	return ctx->devices.touches;
}

DeviceData& device(DeviceId id) {
	return ctx->devices.devices[id];
}

DeviceType deviceType(DeviceId id) {
	return ctx->devices.devices[id].type;
}

void holdDevice(PlayerId playerId, DeviceId deviceId) {
	Player& player = ctx->players[playerId];
	player.heldDevices.emplace_back(deviceId);
}

void releaseDevice(PlayerId playerId, DeviceId deviceId) {
	Player& player = ctx->players[playerId];
	std::erase(player.heldDevices, deviceId);
}

/*
 * Core
 */

void initialize(unsigned int maxPlayers) {
	ctx = new Context{};
	ctx->actions.reserve(32);
	ctx->players.resize(maxPlayers);
	ctx->devices.mice.reserve(8);
	ctx->devices.keyboards.reserve(8);
	ctx->devices.gamepads.reserve(8);
	ctx->devices.touches.reserve(8);
	ctx->devices.devices.fill({DeviceType::None, InvalidDeviceId, nullptr});
}

Context& getContext() {
	return *ctx;
}

void _processTrigger(Trigger& trigger, const ActionValue& value) {
	switch (trigger.type) {
	case TriggerType::Down		: click::down::processEvent(trigger, value); break;
	case TriggerType::Press		: click::press::processEvent(trigger, value); break;
	case TriggerType::Release	: click::release::processEvent(trigger, value); break;
	case TriggerType::Hold		: click::hold::processEvent(trigger, value); break;
	case TriggerType::Continuous: click::continuous::processEvent(trigger, value); break;
	default						: assert(false); break;
	}
}

TriggerState _updateTrigger(Trigger& trigger, float deltaTime) {
	switch (trigger.type) {
	case TriggerType::Down		: return click::down::update(trigger, deltaTime); break;
	case TriggerType::Press		: return click::press::update(trigger, deltaTime); break;
	case TriggerType::Release	: return click::release::update(trigger, deltaTime); break;
	case TriggerType::Hold		: return click::hold::update(trigger, deltaTime); break;
	case TriggerType::Continuous: return click::continuous::update(trigger, deltaTime); break;
	default						: assert(false); break;
	}

	return TriggerState::Idle;
}

void _processModifier(Modifier& modifier, ActionValue& value) {
	switch (modifier.type) {
	case ModifierType::Negate: click::negate::modify(modifier, value); break;
	case ModifierType::ToAxis: click::toAxis::modify(modifier, value); break;
	default					 : assert(false); break;
	}
}

void update(float deltaTime) {
	for (auto& action : ctx->actions) {
		for (auto& state : action.states) {
			state.state = InputState::None;
			state.value = ActionValue{};
		}
	}

	for (auto i = 0; auto& player : ctx->players) {
		for (auto& inputMap : player.inputMaps) {
			for (auto& actionMap : inputMap.actionMaps) {
				auto state = InputState::None;
				auto value = ActionValue{};
				for (auto& gesture : actionMap.gestures) {
					if (gesture.triggers.empty()) {
						if (!_isValueZero(gesture.value)) {
							state = InputState::Triggered;
							value = gesture.value;
							gesture.value = ActionValue{};
						}
						break;
					}

					auto allTriggersSucceded = true;
					for (auto& trigger : gesture.triggers) {
						auto triggerState = _updateTrigger(trigger, deltaTime);
						allTriggersSucceded &= triggerState == TriggerState::Triggered;
					}

					if (allTriggersSucceded) {
						state = InputState::Triggered;
						value.vec3.x += gesture.value.vec3.x;
						value.vec3.y += gesture.value.vec3.y;
						value.vec3.z += gesture.value.vec3.z;
					}
				}

				auto& actionState = ctx->actions[actionMap.actionId].states[i];
				if (actionState.state == InputState::Triggered) {
					continue;
				}
				actionState.state = state;
				actionState.value = value;
			}
		}
		++i;
	}
}

Trigger downTrigger(float actuationThreshold) {
	return Trigger{TriggerType::Down, {.down = {actuationThreshold}}};
}

Trigger pressTrigger(float actuationThreshold) {
	return Trigger{TriggerType::Press, {.press = {actuationThreshold}}};
}

Trigger releaseTrigger(float actuationThreshold) {
	return Trigger{TriggerType::Release, {.release = {actuationThreshold}}};
}

Trigger holdTrigger(float holdTime) {
	return Trigger{TriggerType::Hold, {.hold = {holdTime}}};
}

Trigger continuousTrigger(float actuationThreshold) {
	return Trigger{TriggerType::Continuous, {.continuous = {actuationThreshold}}};
}

Modifier negateModifier() {
	return Modifier{ModifierType::Negate, {}};
}

Modifier toAxisModifier(Axis axis) {
	return Modifier{ModifierType::ToAxis, {.toAxis = axis}};
}

/*
 * Mapping
 *
 */

InputMap* _getInputMap(MapId mapId) {
	for (auto& player : ctx->players) {
		for (auto& inputMap : player.inputMaps) {
			if (inputMap.id == mapId) {
				return &inputMap;
			}
		}
	}
	return nullptr;
}

MapId addInputMap(PlayerId playerId) {
	static MapId mapId = 0;
	auto& player = ctx->players[playerId];
	player.inputMaps.emplace_back(mapId);
	return mapId++;
}

void removeInputMap(MapId mapId) {
	for (auto& player : ctx->players) {
		std::erase_if(player.inputMaps, [mapId](const InputMap& inputMap) { return inputMap.id == mapId; });
	}
}

void disableInputMap(MapId mapId) {
	auto inputMap = _getInputMap(mapId);
	assert(inputMap != nullptr);
	inputMap->isEnabled = false;
}

void enableInputMap(MapId mapId) {
	auto inputMap = _getInputMap(mapId);
	assert(inputMap != nullptr);
	inputMap->isEnabled = true;
}

/*
 * Action
 */

ActionId createAction(ActionType type) {
	ActionId actionId = 0;
	if (ctx->_freeActionIndices.empty()) {
		actionId = static_cast<unsigned int>(ctx->actions.size());
		ctx->actions.emplace_back();
	} else {
		actionId = ctx->_freeActionIndices.back();
		ctx->_freeActionIndices.pop_back();
	}
	auto& action = ctx->actions[actionId];
	action.type = type;
	action.states.resize(ctx->players.size());
	return actionId++;
}

void destroyAction(ActionId actionId) {
	ctx->_freeActionIndices.push_back(actionId);
	ctx->actions[actionId] = {};

	for (auto& player : ctx->players) {
		for (auto& inputMap : player.inputMaps) {
			std::erase_if(inputMap.actionMaps,
						  [actionId](const ActionMapping& actionMap) { return actionMap.actionId == actionId; });
		}
	}
}

ActionType type(ActionId id) {
	return ctx->actions[id].type;
}

unsigned int actions() {
	return static_cast<unsigned int>(ctx->actions.size());
}

const ActionState& actionState(ActionId actionId, PlayerId playerId) {
	auto& state = ctx->actions[actionId].states[playerId];
	return state;
}

/*
 * Gesture
 */

GestureId _bindGesture(InputMap& inputMap,
					   ActionId actionId,
					   Gesture gesture,
					   std::vector<Trigger> triggers,
					   std::vector<Modifier> modifiers) {
	auto itr = std::ranges::find_if(inputMap.actionMaps,
									[actionId](const ActionMapping& item) { return item.actionId == actionId; });
	if (itr == inputMap.actionMaps.end()) {
		inputMap.actionMaps.emplace_back(actionId);
		itr = inputMap.actionMaps.end() - 1;
	}

	static GestureId gestureId = 0;
	itr->gestures.emplace_back(gestureId, gesture, std::move(triggers), std::move(modifiers));
	return gestureId++;
}

DeviceGesture* _getGesture(InputMap* inputMap, GestureId gestureId) {
	for (auto& actionMap : inputMap->actionMaps) {
		for (auto& gesture : actionMap.gestures) {
			if (gesture.gestureId == gestureId) {
				return &gesture;
			}
		}
	}
	return nullptr;
}

DeviceGesture* _getGesture(MapId inputMapId, GestureId gestureId) {
	auto inputMap = _getInputMap(inputMapId);
	assert(inputMap != nullptr);
	return _getGesture(inputMap, gestureId);
}

GestureId bindGesture(MapId inputMapId,
					  ActionId actionId,
					  Gesture gesture,
					  std::vector<Trigger> triggers,
					  std::vector<Modifier> modifiers) {
	auto inputMap = _getInputMap(inputMapId);
	assert(inputMap != nullptr);
	return _bindGesture(*inputMap, actionId, gesture, std::move(triggers), std::move(modifiers));
}

void rebindGesture(MapId inputMapId,
				   GestureId gestureId,
				   Gesture gesture,
				   std::vector<Trigger> triggers,
				   std::vector<Modifier> modifiers) {
	auto gestureToChange = _getGesture(inputMapId, gestureId);
	assert(gestureToChange != nullptr);
	gestureToChange->gesture = gesture;
	gestureToChange->triggers = std::move(triggers);
	gestureToChange->modifiers = std::move(modifiers);
	gestureToChange->value = ActionValue{};
}

void rebindGesture(MapId inputMapId, GestureId gestureId, Gesture gesture, std::vector<Trigger> triggers) {
	auto gestureToChange = _getGesture(inputMapId, gestureId);
	assert(gestureToChange != nullptr);
	gestureToChange->gesture = gesture;
	gestureToChange->triggers = std::move(triggers);
	gestureToChange->value = ActionValue{};
}

void rebindGesture(MapId inputMapId, GestureId gestureId, Gesture gesture) {
	auto gestureToChange = _getGesture(inputMapId, gestureId);
	assert(gestureToChange != nullptr);
	gestureToChange->gesture = gesture;
	gestureToChange->value = ActionValue{};
}

void unbindGesture(GestureId gestureId) {
	for (auto& player : ctx->players) {
		for (auto& inputMap : player.inputMaps) {
			for (auto& actionMap : inputMap.actionMaps) {
				if (std::erase_if(actionMap.gestures, [gestureId](const DeviceGesture& gesture) {
						return gesture.gestureId == gestureId;
					}) > 0) {
					return;
				}
			}
		}
	}
}

/*
 * Input & Event Processing
 */

void updateMouse(DeviceId mouseId, float x, float y) {
	auto xRel = x - ctx->mouseState.x;
	auto yRel = y - ctx->mouseState.y;
	ctx->mouseState.xRel = xRel;
	ctx->mouseState.yRel = yRel;
	ctx->mouseState.x = x;
	ctx->mouseState.y = y;

	processInputEvent({Gesture::MouseX, mouseId, xRel});
	processInputEvent({Gesture::MouseY, mouseId, yRel});
	processInputEvent({Gesture::MouseXY, mouseId, {.vec2 = {xRel, yRel}}});
}

void updateGamepadAxis(DeviceId gamepadId, Gesture axis, ActionValue& value) {
	processInputEvent({axis, gamepadId, value});
}

void processInputEvent(const InputEvent& event) {
	for (auto& player : ctx->players) {
		if (player.heldDevices.empty() ||
			std::ranges::find(player.heldDevices, event.deviceId) == player.heldDevices.end()) {
			continue;
		}

		for (auto& inputMap : player.inputMaps) {
			if (!inputMap.isEnabled) {
				continue;
			}

			for (auto& actionMap : inputMap.actionMaps) {
				for (auto& gesture : actionMap.gestures) {
					if (gesture.gesture == event.gesture) {
						gesture.value = event.value;
						for (auto& modifier : gesture.modifiers) {
							_processModifier(modifier, gesture.value);
						}
						for (auto& trigger : gesture.triggers) {
							_processTrigger(trigger, gesture.value);
						}
					}
				}
			}
		}
	}
}

} // namespace click
