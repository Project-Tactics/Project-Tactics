#pragma once

#include "Json.h"

#include <Libs/Utility/HashId.h>
#include <Libs/Utility/String/String.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <variant>

#define JSON_SERIALIZE(Type, ...)	  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Type, __VA_ARGS__)
#define JSON_SERIALIZE_EXT(Type, ...) NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Type, __VA_ARGS__)
#define JSON_VARIANT_NAME(Type, Name)                 \
	template<> struct VariantJsonUtil<Type> {         \
		static std::string typeName() {               \
			return Name;                              \
		}                                             \
                                                      \
		static bool compare(std::string_view str) {   \
			return variantJsonUtilCompare(str, Name); \
		}                                             \
	}

/*
 * Used mostly to be able to break during debugging
 */
inline bool variantJsonUtilCompare(std::string_view str1, std::string_view str2) {
	return str1 == str2;
}

template<typename T> struct VariantJsonUtil {
	static std::string typeName() {
		static_assert(false, "VariantJsonUtil not implemented for type. Use JSON_VARIANT_NAME macro");
		return "";
	}

	static bool compare(std::string_view) {
		static_assert(false, "VariantJsonUtil not implemented for type. Use JSON_VARIANT_NAME macro");
		return false;
	}
};

namespace nlohmann {

template<typename T>
concept is_enum = std::is_enum_v<T>;

template<is_enum EnumType> struct adl_serializer<EnumType> {
	static void to_json(json& json, const EnumType& value) {
		json = toString(value);
	}

	static void from_json(const json& json, EnumType& value) {
		std::string str = json.get<std::string>();
		value = fromString<EnumType>(str);
	}
};

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

template<typename... Ts> struct adl_serializer<std::variant<Ts...>> {
	static void to_json(json& j, const std::variant<Ts...>& value) {
		std::visit(
			[&j]<typename T>(const T& v) {
				j = v;
				j["type"] = VariantJsonUtil<T>::typeName();
			},
			value);
	}

	template<typename Variant, typename Func, std::size_t Index = 0>
	static bool visit_types(Func&& func, const std::string& type) {
		if constexpr (Index < std::variant_size_v<Variant>) {
			using T = std::variant_alternative_t<Index, Variant>;
			if (VariantJsonUtil<T>::compare(type)) {
				func.template operator()<T>();
				return true;
			} else {
				return visit_types<Variant, Func, Index + 1>(std::forward<Func>(func), type);
			}
		} else {
			return false;
		}
	}

	static void from_json(const json& j, std::variant<Ts...>& value) {
		if (!j.contains("type")) {
			throw TACTICS_EXCEPTION("Failed to deserialize variant type, no [type] field");
		}

		auto type = j["type"].get<std::string>();

		auto success = visit_types<std::variant<Ts...>>([&value, &j]<typename T>() { value = j.get<T>(); }, type);
		if (!success) {
			throw TACTICS_EXCEPTION("Failed to deserialize variant type, {}", type);
		}
	}
};

} // namespace nlohmann
