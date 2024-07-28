#include "InputSerialization.h"

#include <Libs/Utility/Log/Log.h>
#include <Libs/Utility/String/String.h>

namespace click {

void to_json(json&, const Condition&) {}

void from_json(const json& json, Condition& trigger) {
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

void to_json(json&, const Modifier&) {}

void from_json(const json& json, Modifier& modifier) {
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

void to_json(json&, const Gesture&) {}

void from_json(const json& json, Gesture& gesture) {
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

