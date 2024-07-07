#pragma once

#include "String.h"

#include <fmt/format.h>
#include <glm/glm.hpp>

namespace tactics {
template<> class Str<glm::vec2> {
public:
	static std::string to(const glm::vec2& val) {
		return fmt::format("{}, {}", val.x, val.y);
	}

	static glm::vec2 from(std::string_view string) {
		auto array = parseStringToVectorValues<float>(std::string(string));
		return {array[0], array[1]};
	}
};

template<> class Str<glm::vec3> {
public:
	static std::string to(const glm::vec3& val) {
		return fmt::format("{}, {}, {}", val.x, val.y, val.z);
	}

	static glm::vec3 from(std::string_view string) {
		auto array = parseStringToVectorValues<float>(std::string(string));
		return {array[0], array[1], array[2]};
	}
};

template<> class Str<glm::vec4> {
public:
	static std::string to(const glm::vec4& val) {
		return fmt::format("{}, {}, {}, {}", val.x, val.y, val.z, val.w);
	}

	static glm::vec4 from(std::string_view string) {
		auto array = parseStringToVectorValues<float>(std::string(string));
		return {array[0], array[1], array[2], array[3]};
	}
};

} // namespace tactics

FORMAT_STR(glm::vec2);
FORMAT_STR(glm::vec3);
FORMAT_STR(glm::vec4);
FORMAT_STR(glm::ivec2);
FORMAT_STR(glm::ivec3);
FORMAT_STR(glm::ivec4);
