#include "InputSerialization.h"

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
	case ModifierType::ToAxis: modifier.data.toAxis.axis = json["axis"]; break;
	}
}

} // namespace click
