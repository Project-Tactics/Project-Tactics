#pragma once

#include "ClickActionValue.h"
#include "ClickEnumTypes.h"

#include <array>
#include <tuple>
#include <vector>

namespace click {

using ActionId = unsigned int;
using DeviceId = uint8_t;
using PlayerId = uint8_t;
using MapId = uint8_t;
using GestureId = uint16_t;

const auto InvalidDeviceId = std::numeric_limits<uint8_t>::max();

struct InputActionState {
	InputState state{};
	ActionValue value{};
};

struct InputAction {
	ActionType type{};
	std::vector<InputActionState> states;
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
			float holdTime;
		} hold;

		struct {
			float actuationThreshold;
		} continuous;
	} data;

	TriggerState state{};
};

struct Modifier {
	ModifierType type;

	union {
		struct {
		} negate;

		struct {
			Axis axis;
		} toAxis;

	} data;
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
	std::vector<int> _freeDeviceIndices;
};

struct DeviceGesture {
	GestureId gestureId;
	Gesture gesture;
	std::vector<Trigger> triggers;
	std::vector<Modifier> modifiers;
	ActionValue value{};
};

struct ActionMapping {
	ActionId actionId{};
	std::vector<DeviceGesture> gestures;
};

struct InputMap {
	MapId id{};
	std::vector<ActionMapping> actionMaps;
	bool isEnabled{true};
};

struct Player {
	std::vector<InputMap> inputMaps;
	std::vector<DeviceId> heldDevices;
};

struct MousePosition {
	float x{};
	float y{};
	float xRel{};
	float yRel{};
};

struct Context {
	std::vector<InputAction> actions;
	std::vector<Player> players;
	DeviceInfo devices;
	MousePosition mouseState;
	std::vector<int> _freeActionIndices;
};

} // namespace click
