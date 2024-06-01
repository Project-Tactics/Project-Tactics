#pragma once

#include <entt/entt.hpp>

namespace tactics {

class EntityComponentSubSystem {
public:
	EntityComponentSubSystem(entt::registry& registry): _registry(registry) {}
	virtual ~EntityComponentSubSystem() = default;
	virtual void update() = 0;

protected:
	// Every SubSystem is meant to iterate a lot with the entt::registry class so we're
	// decreasing a bit the verbosity by providing a smaller name for it.
	entt::registry& _reg() { return _registry; }

private:
	entt::registry& _registry;
};

}
