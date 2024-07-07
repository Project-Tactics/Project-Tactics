#pragma once

#include "ClickActionValue.h"
#include "ClickEnumTypes.h"

#include <array>
#include <tuple>
#include <vector>

namespace click {

using InputActionId = unsigned int;
using DeviceId = uint8_t;
using OwnerId = uint8_t;

const auto InvalidDeviceId = std::numeric_limits<uint8_t>::max();

struct InputAction {
	InputActionId id{};
	ActionType type{};
	InputState state{};
	ActionValue value{};
};

union DeviceGestureData {
	Key key;
	MouseGesture mouse;
	GamepadGesture gamepad;
	TouchGesture touch;
	int32_t other{};
};

struct Trigger {
	TriggerType type;

	union {
		struct {
			float actuationThreshold;
		} down;

		struct {
			float actuationThreshold;
		} press;

		struct {
			float actuationThreshold;
		} release;

		struct {
			float actuationThreshold;
			float holdTime;
		} hold;
	} data;

	TriggerState state{};
};

struct Modifier {
	ModifierType type;

	union {
		struct {
		} negate;

	} data;
};

struct DeviceGesture {
	DeviceId deviceId;
	DeviceGestureData gesture;
	std::vector<Trigger> triggers;
	std::vector<Modifier> modifiers;
	ActionValue value{};
};

struct Mapping {
	InputActionId actionId{};
	std::vector<DeviceGesture> gestures;
};

struct DeviceData {
	DeviceType type{DeviceType::None};
	DeviceId deviceId{InvalidDeviceId};
	void* customData{};
};

using DeviceList = std::array<DeviceData, 16>;
using DeviceIdList = std::vector<DeviceId>;

struct DeviceInfo {
	DeviceIdList mice;
	DeviceIdList keyboards;
	DeviceIdList gamepads;
	DeviceIdList touches;

	DeviceList devices;
};

struct Context {
	std::vector<OwnerId> owners;
	std::vector<InputAction> actions;
	std::vector<Mapping> mappings;
	DeviceInfo devices;
};

} // namespace click
