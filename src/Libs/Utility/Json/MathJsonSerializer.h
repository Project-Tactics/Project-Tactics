#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <nlohmann/json.hpp>

namespace nlohmann {
template<> struct adl_serializer<glm::vec4> {
	static void to_json(json& j, const glm::vec4& value);
	static void from_json(const json& j, glm::vec4& value);
};

template<> struct adl_serializer<glm::vec3> {
	static void to_json(json& j, const glm::vec3& value);
	static void from_json(const json& j, glm::vec3& value);
};

template<> struct adl_serializer<glm::vec2> {
	static void to_json(json& j, const glm::vec2& value);
	static void from_json(const json& j, glm::vec2& value);
};

template<> struct adl_serializer<glm::quat> {
	static void to_json(json& j, const glm::quat& value);
	static void from_json(const json& j, glm::quat& value);
};

template<> struct adl_serializer<glm::mat4> {
	static void to_json(json& j, const glm::mat4& value);
	static void from_json(const json& j, glm::mat4& value);
};
} // namespace nlohmann
