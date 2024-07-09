#pragma once

#include "ClickInputTypes.h"

namespace click {

struct InputEvent {
	Gesture gesture{};
	DeviceId deviceId{};
	ActionValue value{};
};

} // namespace click
