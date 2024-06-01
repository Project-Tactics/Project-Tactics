#include "EntityUtilities.h"

namespace tactics {

#if defined(_DEBUG)
hash_string hash(const char* str) {
	return entt::hashed_string(str);
}

const char* toString(const hash_string& hash) {
	return hash.data();
}

#else

hash_string hash(const char* str) {
	return entt::hashed_string::value(str);
}

const char* toString(const hash_string& hash) {
	return "NO_ID";
}

#endif

}
