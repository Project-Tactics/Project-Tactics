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

void to_json(nlohmann::json&, const Condition&) {}

void from_json(const nlohmann::json& json, Condition& trigger) {
	if (!json.contains("type")) {
		LOG_ERROR(tactics::Log::Input, "Input Modifier missing [type]");
		return;
	}

	std::string type = json["type"];
	if (type == "down") {
		trigger = json.get<DownCondition>();
	} else if (type == "press") {
		trigger = json.get<PressCondition>();
	} else if (type == "release") {
		trigger = json.get<ReleaseCondition>();
	} else if (type == "hold") {
		trigger = json.get<HoldCondition>();
	} else if (type == "continuous") {
		trigger = json.get<ContinuousCondition>();
	} else {
		LOG_ERROR(tactics::Log::Input, "Unknown trigger type: {}", type);
	}
}

void to_json(nlohmann::json&, const Modifier&) {}

void from_json(const nlohmann::json& json, Modifier& modifier) {
	if (!json.contains("type")) {
		LOG_ERROR(tactics::Log::Input, "Input Modifier missing [type]");
		return;
	}

	if (!json.contains("data")) {
		LOG_ERROR(tactics::Log::Input, "Input Modifier missing field [data]");
		return;
	}

	std::string type = json["type"];
	auto& data = json["data"];
	if (type == "negate") {
		modifier = data.get<NegateModifier>();
	} else {
		LOG_ERROR(tactics::Log::Input, "Unknown modifier type: {}", type);
	}
}

void to_json(nlohmann::json&, const Gesture&) {}

void from_json(const nlohmann::json& json, Gesture& gesture) {
	if (!json.contains("type")) {
		LOG_ERROR(tactics::Log::Input, "Input Modifier missing [type]");
		return;
	}

	if (!json.contains("data")) {
		LOG_ERROR(tactics::Log::Input, "Input Modifier missing field [data]");
		return;
	}

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
