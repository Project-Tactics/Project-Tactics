#pragma once

#include "String.h"

#include <fmt/format.h>
#include <glm/glm.hpp>

template<> inline std::string toString(const glm::vec2& val) {
	return fmt::format("{}, {}", val.x, val.y);
}

template<> inline void fromString(const std::string& str, glm::vec2& result) {
	auto array = tactics::parseStringToVectorValues<float>(str);
	result = {array[0], array[1]};
}

template<> inline std::string toString(const glm::vec3& val) {
	return fmt::format("{}, {}, {}", val.x, val.y, val.z);
}

template<> inline void fromString(const std::string& str, glm::vec3& result) {
	auto array = tactics::parseStringToVectorValues<float>(str);
	result = {array[0], array[1], array[2]};
}

template<> inline std::string toString(const glm::vec4& val) {
	return fmt::format("{}, {}, {}, {}", val.x, val.y, val.z, val.w);
}

template<> inline void fromString(const std::string& str, glm::vec4& result) {
	auto array = tactics::parseStringToVectorValues<float>(str);
	result = {array[0], array[1], array[2], array[3]};
}

FORMAT_STR(glm::vec2);
FORMAT_STR(glm::vec3);
FORMAT_STR(glm::vec4);
FORMAT_STR(glm::ivec2);
FORMAT_STR(glm::ivec3);
FORMAT_STR(glm::ivec4);
