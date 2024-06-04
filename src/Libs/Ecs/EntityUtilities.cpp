#include "EntityUtilities.h"

namespace tactics {

#if defined(_DEBUG)
hash_string hash(const char* str) {
	return entt::hashed_string(str);
}

hash_string hash(std::string_view str) {
	return entt::hashed_string(str.data(), str.size());
}

hash_string hash(const std::string& str) {
	return entt::hashed_string(str.c_str());
}

const char* toString(const hash_string& hash) {
	return hash.data();
}

#else

hash_string hash(const char* str) {
	return entt::hashed_string::value(str);
}

hash_string hash(std::string_view str) {
	return entt::hashed_string::value(str.data(), str.size());
}

hash_string hash(const std::string& str) {
	return entt::hashed_string::value(str.c_str());
}

const char* toString(const hash_string& hash) {
	return "NO_ID";
}

#endif

}
