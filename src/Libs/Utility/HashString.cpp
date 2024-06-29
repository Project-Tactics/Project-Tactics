#include "HashString.h"

#include "Libs/Utility/Log/Log.h"

#include <unordered_map>

namespace tactics {

// The entt hashed_string doesn't store the string and that's going to be an issue for all temp strings created.
// We're going to store the string in a map so we can retrieve it later.
// TODO(Gerark) Probably we don't need this in a non-debug build ( Measure first? ).
std::unordered_map<hash_string::hash_type, std::string> hashToStringMap;

hash_string hash(const char* str) {
	auto hash = entt::hashed_string(str);
	if (auto itr = hashToStringMap.find(hash.value()); itr != hashToStringMap.end()) {
		if (itr->second != str) {
			LOG_CRITICAL(Log::Engine, "Hash collision detected for string '{}'. There's another string with the same hash: {}", str, itr->second);
		}
	}
	hashToStringMap[hash.value()] = str;
	return hash;
}

hash_string hash(std::string_view str) {
	return hash(str.data());
}

hash_string hash(const std::string& str) {
	return hash(str.c_str());
}

const char* toString(const hash_string& hash) {
	if (auto itr = hashToStringMap.find(hash.value()); itr != hashToStringMap.end()) {
		return itr->second.c_str();
	}
	LOG_ERROR(Log::Engine, "Trying to get the string for hash {} but it doesn't exist. Be sure to create all hashed string through the tactics::hash method", hash.value());
	return "";
}

hash_string none_hash() {
	return entt::hashed_string("");
}

}
