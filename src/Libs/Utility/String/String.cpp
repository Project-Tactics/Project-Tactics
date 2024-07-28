#include "String.h"

#include <algorithm>

namespace tactics {

char tolower(char c) {
	return static_cast<char>(::tolower(static_cast<int>(c)));
}

char toupper(char c) {
	return static_cast<char>(::toupper(static_cast<int>(c)));
}

void tolower(std::string& str) {
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return tolower(c); });
}

void toupper(std::string& str) {
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return toupper(c); });
}

void tolower(char* str) {
	while (*str) {
		*str = tolower(*str);
		++str;
	}
}

void toupper(char* str) {
	while (*str) {
		*str = toupper(*str);
		++str;
	}
}

} // namespace tactics
