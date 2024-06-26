#pragma once

#include <Libs/Utility/HashId.h>
#include <Libs/Utility/Service/ServiceLocator.h>

#include <entt/entt.hpp>

namespace tactics {
template<typename... TComponents> using ecs_view = const entt::view<entt::get_t<TComponents...>>;

}
