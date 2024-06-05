#pragma once

#include <entt/entt.hpp>

namespace tactics {

using hash_string = entt::hashed_string;

hash_string hash(const char* str);
hash_string hash(const std::string& str);
hash_string hash(std::string_view str);

// Always prefer the use of toString to get the internal string of a hash_string. hash_string::data() might return a pointer to a temporary string ( dangled pointer )
const char* toString(const hash_string& hash);
hash_string none_hash();

template<typename ...TComponents>
using ecs_view = const entt::view<entt::get_t<TComponents...>>;

}
