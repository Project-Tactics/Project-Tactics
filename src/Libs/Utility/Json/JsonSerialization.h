#pragma once

#include "Json.h"

#include <Libs/Utility/HashId.h>
#include <Libs/Utility/String/String.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

template<typename BasicJsonType, typename EnumType>
inline void enum_from_json(const BasicJsonType& json, EnumType& value) {
	std::string str = json.get<std::string>();
	value = fromString<EnumType>(str);
}

template<typename BasicJsonType, typename EnumType>
inline void enum_to_json(BasicJsonType& json, const EnumType& value) {
	json = toString(value);
}

#define JSON_SERIALIZE(Type, ...)	  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Type, __VA_ARGS__)
#define JSON_SERIALIZE_EXT(Type, ...) NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Type, __VA_ARGS__)
#define JSON_ENUM(ENUM_TYPE)                                                                              \
	template<typename BasicJsonType> inline void to_json(BasicJsonType& json, const ENUM_TYPE& value) {   \
		static_assert(std::is_enum<ENUM_TYPE>::value, #ENUM_TYPE " must be an enum!");                    \
		enum_to_json(json, value);                                                                        \
	}                                                                                                     \
                                                                                                          \
	template<typename BasicJsonType> inline void from_json(const BasicJsonType& json, ENUM_TYPE& value) { \
		static_assert(std::is_enum<ENUM_TYPE>::value, #ENUM_TYPE " must be an enum!");                    \
		enum_from_json(json, value);                                                                      \
	}

namespace nlohmann {
template<> struct adl_serializer<tactics::HashId> {
	static void to_json(json& j, const tactics::HashId& value);
	static void from_json(const json& j, tactics::HashId& value);
};

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
