#pragma once

#include <entt/entt.hpp>

namespace tactics {

#if defined(_DEBUG)
using hash_string = entt::hashed_string;
#else
using hash_string = entt::hashed_string::hash_type;
#endif

hash_string hash(const char* str);
hash_string hash(const std::string& str);
hash_string hash(std::string_view str);
const char* toString(const hash_string& hash);

template<typename ...TComponents>
using ecs_view = const entt::view<entt::get_t<TComponents...>>;

}
