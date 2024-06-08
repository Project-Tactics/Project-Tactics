#pragma once

#include <entt/entt.hpp>

namespace tactics {

class ComponentReflection {
public:
	static void InitializeBasicTypes(entt::registry& registry);
	static void InitializeCommonComponents(entt::registry& registry);
};

}
