#pragma once

#include <entt/entt.hpp>

namespace tactics {

class Scene {
public:
	entt::registry& getRegistry() {
		return _registry;
	}

private:
	entt::registry _registry;
};

}
