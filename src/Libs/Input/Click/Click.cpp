#include "Click.h"

#include "ClickModifiers.h"
#include "ClickTriggers.h"

#include <assert.h>

namespace click {

Context* ctx = nullptr;

auto _isValueZero(const ActionValue& value) {
	return value.axis3D.x == 0.0f && value.axis3D.y == 0.0f && value.axis3D.z == 0.0f;
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

/*
 * Core
 */

void initializeContext() {
	ctx = new Context{};
	ctx->actions.reserve(32);
	ctx->mappings.reserve(32);
	ctx->owners.reserve(32);
	ctx->devices.mice.reserve(8);
	ctx->devices.keyboards.reserve(8);
	ctx->devices.gamepads.reserve(8);
	ctx->devices.touches.reserve(8);
	ctx->devices.devices.fill({DeviceType::None, InvalidDeviceId, nullptr});
}

Context& getContext() {
	return *ctx;
}

InputActionId createAction(ActionType type) {
	auto id = static_cast<unsigned int>(ctx->actions.size());
	ctx->actions.emplace_back(id, type);
	ctx->mappings.emplace_back(Mapping{});
	return id;
}

OwnerId owner() {
	auto ownerId = static_cast<OwnerId>(ctx->owners.size());
	ctx->owners.push_back(ownerId);
	return ownerId;
}

InputActionId impulse() {
	return createAction(ActionType::Impulse);
}

InputActionId axis1D() {
	return createAction(ActionType::Axis1D);
}

InputActionId axis2D() {
	return createAction(ActionType::Axis2D);
}

InputActionId axis3D() {
	return createAction(ActionType::Axis3D);
}

ActionType type(InputActionId id) {
	return ctx->actions[id].type;
}

std::tuple<InputState, ActionValue> _inputState(InputActionId actionId, ActionType type) {
	assert(type == ctx->actions[actionId].type);
	return {ctx->actions[actionId].state, ctx->actions[actionId].value};
}

std::tuple<InputState, bool> impulseState(InputActionId id) {
	auto [state, value] = _inputState(id, ActionType::Impulse);
	return {state, value.axis1D != 0.0f};
}

std::tuple<InputState, float> axis1DState(InputActionId id) {
	auto [state, value] = _inputState(id, ActionType::Axis1D);
	return {state, value.axis1D};
}

std::tuple<InputState, Axis2D> axis2DState(InputActionId id) {
	auto [state, value] = _inputState(id, ActionType::Axis2D);
	return {state, value.axis2D};
}

std::tuple<InputState, Axis3D> axis3DState(InputActionId id) {
	auto [state, value] = _inputState(id, ActionType::Axis3D);
	return {state, value.axis3D};
}

void _processTrigger(Trigger& trigger, const ActionValue& value) {
	switch (trigger.type) {
	case TriggerType::Down	 : click::down::processEvent(trigger, value); break;
	case TriggerType::Press	 : click::press::processEvent(trigger, value); break;
	case TriggerType::Release: click::release::processEvent(trigger, value); break;
	case TriggerType::Hold	 : click::hold::processEvent(trigger, value); break;
	default					 : assert(false); break;
	}
}

TriggerState _updateTrigger(Trigger& trigger, float deltaTime) {
	switch (trigger.type) {
	case TriggerType::Down	 : return click::down::update(trigger, deltaTime); break;
	case TriggerType::Press	 : return click::press::update(trigger, deltaTime); break;
	case TriggerType::Release: return click::release::update(trigger, deltaTime); break;
	case TriggerType::Hold	 : return click::hold::update(trigger, deltaTime); break;
	default					 : assert(false); break;
	}

	return TriggerState::Idle;
}

void _processModifier(Modifier& modifier, ActionValue& value) {
	switch (modifier.type) {
	case ModifierType::Negate: click::negate::modify(modifier, value); break;
	default					 : assert(false); break;
	}
}

void update(float deltaTime) {
	for (auto actionId = 0; auto& mapping : ctx->mappings) {
		auto state = InputState::None;
		auto value = ActionValue{};
		for (auto& gesture : mapping.gestures) {
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
				value = gesture.value;
				break;
			}
		}

		ctx->actions[actionId].state = state;
		ctx->actions[actionId].value = value;
		++actionId;
	}
}

void processKeyboardEvent(const KeyboardEvent& event) {
	for (auto& mapping : ctx->mappings) {
		for (auto& gesture : mapping.gestures) {
			if (gesture.deviceId == event.deviceId && gesture.gesture.key == event.key) {
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

void processGamepadEvent(const GamepadEvent& event) {
	for (auto& mapping : ctx->mappings) {
		for (auto& gesture : mapping.gestures) {
			if (gesture.deviceId == event.deviceId && gesture.gesture.gamepad == event.gesture) {
				gesture.value = event.value;
				for (auto& trigger : gesture.triggers) {
					_processTrigger(trigger, gesture.value);
				}
			}
		}
	}
}

void processMouseEvent(const MouseEvent& event) {
	for (auto& mapping : ctx->mappings) {
		for (auto& gesture : mapping.gestures) {
			if (gesture.deviceId == event.deviceId && gesture.gesture.mouse == event.gesture) {
				gesture.value = event.value;
				for (auto& trigger : gesture.triggers) {
					_processTrigger(trigger, gesture.value);
				}
			}
		}
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

Trigger holdTrigger(float actuationThreshold, float holdTime) {
	return Trigger{TriggerType::Hold, {.hold = {actuationThreshold, holdTime}}};
}

Modifier negateModifier() {
	return Modifier{ModifierType::Negate, {}};
}

void map(InputActionId actionId,
		 DeviceId deviceId,
		 const DeviceGestureData& gestureData,
		 std::vector<Trigger> triggers,
		 std::vector<Modifier> modifiers) {
	auto& mapping = ctx->mappings[actionId];
	mapping.gestures.emplace_back(deviceId, gestureData, std::move(triggers), std::move(modifiers));
}

} // namespace click
