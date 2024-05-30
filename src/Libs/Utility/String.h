#pragma once

#include <string>

namespace tactics {

char tolower(char c);
char toupper(char c);

template<typename T>
class Str {
public:
	static std::string to(T value) {
		return std::to_string(value);
	}

	static T from(std::string_view value) {
		return default(T);
	}
};

template<typename T>
std::string toString(const T& value) {
	return Str<T>::to(value);
}

template<typename T>
T fromString(std::string_view value) {
	return Str<T>::from(value);
}

}
