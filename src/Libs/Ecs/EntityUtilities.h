#pragma once

#include <entt/entt.hpp>

namespace tactics {
template<typename... TComponents> using ecs_view = const entt::view<entt::get_t<TComponents...>>;

}
