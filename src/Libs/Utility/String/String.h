#pragma once

#include "../Exception.h"

#include <array>
#include <magic_enum.hpp>
#include <sstream>
#include <string>
#include <string_view>

template<typename T> std::string toString(const T& value) {
	if constexpr (std::is_enum<T>::value) {
		return std::string(magic_enum::enum_name(value));
	} else {
		return std::to_string(value);
	}
}

template<typename T> void fromString(const std::string& strValue, T& result) {
	if constexpr (std::is_enum<T>::value) {
		if (auto value = magic_enum::enum_cast<T>(strValue); value.has_value()) {
			result = *value;
		} else {
			throw TACTICS_EXCEPTION("Cannot convert string {} to type {}", strValue, magic_enum::enum_type_name<T>());
		}
	} else {
		std::istringstream ss(strValue);
		if (!(ss >> result)) {
			throw TACTICS_EXCEPTION("Cannot convert string to type T");
		}
	}
}

template<typename T> T fromString(const std::string& str) {
	T result;
	fromString(str, result);
	return result;
}

namespace tactics {

char tolower(char c);
char toupper(char c);
void tolower(std::string& str);
void toupper(std::string& str);
void tolower(char* str);
void toupper(char* str);

template<typename T> std::array<T, 4> parseStringToVectorValues(const std::string& str) {
	std::array<T, 4> result = {};
	std::stringstream ss(str);
	std::string temp;
	size_t index = 0;

	while (std::getline(ss, temp, ',') && index < result.size()) {
		result[index++] = fromString<T>(temp);
	}

	return result;
}

} // namespace tactics

#define FORMAT_STR(TYPE)                                                                        \
	template<> struct ::fmt::formatter<TYPE> {                                                  \
	public:                                                                                     \
		constexpr auto parse(::fmt::format_parse_context& ctx) {                                \
			return ctx.begin();                                                                 \
		}                                                                                       \
                                                                                                \
		template<typename Context> constexpr auto format(TYPE const& obj, Context& ctx) const { \
			return ::fmt::format_to(ctx.out(), "{}", toString(obj));                            \
		}                                                                                       \
	};
