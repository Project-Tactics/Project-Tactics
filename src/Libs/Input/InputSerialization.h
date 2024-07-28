#pragma once

#include <Libs/Input/Click/Click.h>
#include <Libs/Utility/Json/JsonSerialization.h>

template<> struct magic_enum::customize::enum_range<click::InputCode> {
	static constexpr int min = 0;
	static constexpr int max = 300;
};

template<> inline std::string toString(const click::ActionValue& value) {
	return fmt::format("{:.3f} {:.3f} {:.3f}", value.vec3.x, value.vec3.y, value.vec3.z);
}

template<> inline void fromString(const std::string& str, click::ActionValue& result) {
	auto values = tactics::parseStringToVectorValues<float>(str);
	result = click::ActionValue{.vec3 = {values[0], values[1], values[2]}};
}

namespace click {

void to_json(json&, const Condition&);
void from_json(const json& json, Condition& trigger);
void to_json(json&, const Modifier&);
void from_json(const json& json, Modifier& modifier);
void to_json(json&, const Gesture&);
void from_json(const json& json, Gesture& gesture);

JSON_ENUM(Axis)
JSON_ENUM(ActionType)
JSON_ENUM(InputCode)
JSON_ENUM(DeviceType)
JSON_ENUM(InputState)

JSON_SERIALIZE_EXT(GestureSimple, input);
JSON_SERIALIZE_EXT(Gesture2D, x, y);
JSON_SERIALIZE_EXT(Gesture3D, x, y, z);
JSON_SERIALIZE_EXT(GestureDirectional2D, left, right, down, up);
JSON_SERIALIZE_EXT(GestureDirectional3D, left, right, down, up, back, forward);

JSON_SERIALIZE_EXT(DownCondition, actuationThreshold);
JSON_SERIALIZE_EXT(PressCondition, actuationThreshold);
JSON_SERIALIZE_EXT(ReleaseCondition, actuationThreshold, state);
JSON_SERIALIZE_EXT(HoldCondition, actuationThreshold, holdTime, state);
JSON_SERIALIZE_EXT(ContinuousCondition, actuationThreshold, state);

JSON_SERIALIZE_EXT(NegateModifier, axis);

} // namespace click

FORMAT_STR(click::Axis)
FORMAT_STR(click::ActionType)
FORMAT_STR(click::InputCode)
FORMAT_STR(click::DeviceType)
FORMAT_STR(click::InputState)
FORMAT_STR(click::ActionValue)
