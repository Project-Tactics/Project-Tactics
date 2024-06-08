#pragma once

#include <entt/entt.hpp>

namespace tactics {

class ComponentReflection {
public:
	static void initializeBasicTypes();
	static void initializeCommonComponents();
};

}
