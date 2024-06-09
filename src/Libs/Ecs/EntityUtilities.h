#pragma once

#include <Libs/Utility/Service/ServiceLocator.h>

#include <entt/entt.hpp>

namespace tactics {
using hash_string = entt::hashed_string;

[[nodiscard]] hash_string hash(const char* str);
[[nodiscard]] hash_string hash(const std::string& str);
[[nodiscard]] hash_string hash(std::string_view str);

// Always prefer the use of toString to get the internal string of a hash_string. hash_string::data() might return a pointer to a temporary string ( dangled pointer )
[[nodiscard]] const char* toString(const hash_string& hash);
[[nodiscard]] hash_string none_hash();

template<typename ...TComponents>
using ecs_view = const entt::view<entt::get_t<TComponents...>>;

}
