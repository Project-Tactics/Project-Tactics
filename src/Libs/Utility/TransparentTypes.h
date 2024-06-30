#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

namespace tactics {

/*
 * @brief A group of utils used to work with std::string_view and std containers
 * look at: https://shorturl.at/lmUW7
 */

class TransparentString : public std::string {
public:
	using std::string::string;

	TransparentString(std::string_view view) : std::string(view) {}

	TransparentString(const std::string& s) : std::string(s) {}

	TransparentString(std::string&& s) : std::string(std::move(s)) {}

	TransparentString(const TransparentString&) = default;
	TransparentString(TransparentString&&) = default;
};

struct TransparentStringHash : std::hash<std::string_view> {
	using is_transparent = void;
};

struct TransparentStringEquals : std::equal_to<std::string_view> {
	using is_transparent = void;
};

struct TransparentStringLess : std::less<std::string_view> {
	using is_transparent = void;
};

template<typename T>
using UnorderedStringMap = std::unordered_map<TransparentString, T, TransparentStringHash, TransparentStringEquals>;

template<typename T> void assign(const std::unordered_map<std::string, T>& source, UnorderedStringMap<T>& dest) {
	dest.clear();
	for (auto& [key, value] : source) { dest[key] = value; }
}

} // namespace tactics
