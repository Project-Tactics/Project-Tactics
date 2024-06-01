#pragma once

#include <inicpp.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>

#include <array>

namespace tactics {

template<typename T>
std::array<T, 4> parseIniStringToVectorValues(const std::string& str) {
	std::array<T, 4> result = {};
	std::stringstream ss(str);
	std::string temp;
	size_t index = 0;

	while (std::getline(ss, temp, ',') && index < result.size()) {
		ini::Convert<T>().decode(temp, result[index++]);
	}

	return result;
}

}

// Most probably we could have a better approach by defining the specialized classes for each glm type but
// it's going to take a lot of time.

#define INI_CONVERT_VEC2(OBJECT_TYPE, FIELD_TYPE) \
template<> \
struct ini::Convert<OBJECT_TYPE> { \
	void decode(const std::string& value, OBJECT_TYPE& result) { \
		auto values = tactics::parseIniStringToVectorValues<FIELD_TYPE>(value); \
		result.x = values[0]; \
		result.y = values[1]; \
	} \
	\
	void encode(const OBJECT_TYPE& value, std::string& result) { \
		result = std::to_string(value.x) + "," + std::to_string(value.y); \
	} \
}; \

#define INI_CONVERT_VEC3(OBJECT_TYPE, FIELD_TYPE) \
template<> \
struct ini::Convert<OBJECT_TYPE> { \
	void decode(const std::string& value, OBJECT_TYPE& result) { \
		auto values = tactics::parseIniStringToVectorValues<FIELD_TYPE>(value); \
		result.x = values[0]; \
		result.y = values[1]; \
		result.z = values[2]; \
	} \
	\
	void encode(const OBJECT_TYPE& value, std::string& result) { \
		result = std::to_string(value.x) + "," + std::to_string(value.y) + "," + std::to_string(value.z); \
	} \
}; \

#define INI_CONVERT_VEC4(OBJECT_TYPE, FIELD_TYPE) \
template<> \
struct ini::Convert<OBJECT_TYPE> { \
	void decode(const std::string& value, OBJECT_TYPE& result) { \
		auto values = tactics::parseIniStringToVectorValues<FIELD_TYPE>(value); \
		result.x = values[0]; \
		result.y = values[1]; \
		result.z = values[2]; \
		result.w = values[3]; \
	} \
	\
	void encode(const OBJECT_TYPE& value, std::string& result) { \
		result = std::to_string(value.x) + "," + std::to_string(value.y) + "," + std::to_string(value.z) + "," + std::to_string(value.w); \
	} \
}; \

INI_CONVERT_VEC2(glm::vec2, float);
INI_CONVERT_VEC2(glm::u32vec2, uint32_t);
INI_CONVERT_VEC2(ImVec2, float);

INI_CONVERT_VEC3(glm::vec3, float);
INI_CONVERT_VEC3(glm::u32vec3, uint32_t);

INI_CONVERT_VEC4(glm::vec4, float);
INI_CONVERT_VEC4(glm::u32vec4, uint32_t);
INI_CONVERT_VEC4(ImVec4, float);
