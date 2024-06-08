#pragma once

#include <entt/entt.hpp>

namespace tactics {

class ComponentReflection {
public:
	static void initializeBasicTypes(entt::registry& registry);
	static void initializeCommonComponents(entt::registry& registry);
};

}
