#pragma once

#include "../EntityUtilities.h"

namespace tactics::component {

class BillboardSystem {
public:
	static void update(entt::registry& registry);
};

} // namespace tactics::component
