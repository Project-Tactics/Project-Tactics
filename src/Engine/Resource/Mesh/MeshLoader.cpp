#include "MeshLoader.h"

#include <regex>
#include <functional>

namespace tactics {

template<typename T>
std::vector<T> parseString(const std::string& str, std::function<T(const std::string&)> convertFunc) {
	std::vector<T> result;
	std::regex re("\\s*,\\s*"); // Regular expression to split on commas with optional whitespace around them
	std::sregex_token_iterator it(str.begin(), str.end(), re, -1);
	std::sregex_token_iterator end;

	for (; it != end; ++it) {
		if (!it->str().empty()) {
			result.push_back(convertFunc(it->str()));
		}
	}
	return result;
}

std::vector<float> MeshLoader::parseVertices(const std::string& strVertices) {
	return parseString<float>(strVertices, [] (const std::string& str) { return std::stof(str); });

}

std::vector<unsigned int> MeshLoader::parseIndices(const std::string& strIndices) {
	return parseString<unsigned int>(strIndices, [] (const std::string& str) { return std::stoul(str); });
}

}
