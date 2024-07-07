#pragma once

#include "ClickInputTypes.h"

namespace click {

struct KeyboardEvent {
	Key key{};
	DeviceId deviceId{};
	ActionValue value{};
};

struct MouseEvent {
	MouseGesture gesture{};
	DeviceId deviceId{};
	ActionValue value{};
};

struct GamepadEvent {
	GamepadGesture gesture{};
	DeviceId deviceId{};
	ActionValue value{};
};

} // namespace click
