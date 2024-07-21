#pragma once

#include "ClickInputTypes.h"

namespace click {

struct InputEvent {
	InputCode inputCode{};
	DeviceId deviceId{};
	ActionValue value{};
};

} // namespace click
