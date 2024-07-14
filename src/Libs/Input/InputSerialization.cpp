#include "InputSerialization.h"

#include <Libs/Utility/Log/Log.h>

namespace tactics {

std::string Str<click::DeviceType>::to(click::DeviceType deviceType) {
	switch (deviceType) {
	case click::DeviceType::Keyboard: return "keyboard";
	case click::DeviceType::Mouse	: return "mouse";
	case click::DeviceType::Gamepad : return "gamepad";
	}

	return "none";
}

click::DeviceType Str<click::DeviceType>::from(std::string_view str) {
	if (str == "keyboard") {
		return click::DeviceType::Keyboard;
	}
	if (str == "mouse") {
		return click::DeviceType::Mouse;
	}
	if (str == "gamepad") {
		return click::DeviceType::Gamepad;
	}

	return click::DeviceType::None;
}

} // namespace tactics

namespace click {

void to_json(nlohmann::json&, const Trigger&) {}

void from_json(const nlohmann::json& json, Trigger& trigger) {
	trigger.type = json["type"];
	switch (trigger.type) {
	case TriggerType::Down		: trigger.data.down.actuationThreshold = json["actuationThreshold"]; break;
	case TriggerType::Press		: trigger.data.press.actuationThreshold = json["actuationThreshold"]; break;
	case TriggerType::Release	: trigger.data.release.actuationThreshold = json["actuationThreshold"]; break;
	case TriggerType::Hold		: trigger.data.hold.holdTime = json["holdTime"]; break;
	case TriggerType::Continuous: trigger.data.continuous.actuationThreshold = json["actuationThreshold"]; break;
	}
}

void to_json(nlohmann::json&, const Modifier&) {}

void from_json(const nlohmann::json& json, Modifier& modifier) {
	modifier.type = json["type"];
	switch (modifier.type) {
	case ModifierType::Negate: break;
	}
}

void to_json(nlohmann::json&, const Gesture&) {}

void from_json(const nlohmann::json& json, Gesture& gesture) {
	std::string type = json["type"];
	auto& data = json["data"];
	if (type == "simple") {
		gesture = data.get<GestureSimple>();
	} else if (type == "2d") {
		gesture = data.get<Gesture2D>();
	} else if (type == "3d") {
		gesture = data.get<Gesture3D>();
	} else if (type == "dir2D") {
		gesture = data.get<GestureDirectional2D>();
	} else if (type == "dir3D") {
		gesture = data.get<GestureDirectional3D>();
	} else {
		LOG_ERROR(tactics::Log::Input, "Unknown gesture type: {}", type);
	}
}

} // namespace click
