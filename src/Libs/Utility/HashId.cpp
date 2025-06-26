#include "HashId.h"

#include "Libs/Utility/Log/Log.h"

#include <entt/entt.hpp>
#include <unordered_map>

namespace tactics {

// A simple implementation to store the string associated with a hash.
// It's mainly used to check if there are any collisions in the hash generation to avoid surprising bugs which are quite
// hard to catch. In theory we could avoid to use this map in Release builds to avoid the overhead of storing and
// looking for string but it's just better to measure the performance impact before making any decision.
[[nodiscard]] std::unordered_map<std::uint32_t, std::string>& hashToStringMap() {
	static std::unordered_map<std::uint32_t, std::string> hashToStringMap;
	return hashToStringMap;
}

const HashId HashId::none = HashId("");

HashId::HashId(const char* str) {
	auto hash = entt::hashed_string(str).value();
	auto& map = hashToStringMap();
	if (auto itr = map.find(hash); itr != map.end()) {
		if (itr->second != str) {
			LOG_CRITICAL(Log::Engine,
						 "Hash collision detected for string '{}'. There's another string with the same hash: {}",
						 str,
						 itr->second);
		}
	}
	map[hash] = str;
	_id = hash;
#ifdef _DEBUG
	_debugName = map[hash];
#endif
}

HashId::HashId(const std::string& str) : HashId(str.c_str()) {}

HashId::HashId(std::string_view str) : HashId(str.data()) {}

[[nodiscard]] std::uint32_t HashId::id() const {
	return _id;
}

[[nodiscard]] const char* HashId::str() const {
	if (auto itr = hashToStringMap().find(_id); itr != hashToStringMap().end()) {
		return itr->second.c_str();
	}
	LOG_ERROR(Log::Engine,
			  "Trying to get the string for hash {} but it doesn't exist. Be sure to create all hashed string through "
			  "the tactics::hash method",
			  _id);
	return "";
}

[[nodiscard]] bool HashId::operator==(const HashId& other) const {
	return _id == other._id;
}

[[nodiscard]] bool HashId::operator>(const HashId& other) const {
	return _id > other._id;
}

[[nodiscard]] bool HashId::isEmpty() const {
	return _id == 0 || _id == HashId::none;
}

[[nodiscard]] HashId::operator std::uint32_t() const {
	return _id;
}

HashId operator"" _id(const char* value, size_t) {
	return HashId(value);
}

} // namespace tactics
