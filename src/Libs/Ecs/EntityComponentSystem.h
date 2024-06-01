#pragma once

#include <entt/entt.hpp>

using namespace entt::literals;

namespace tactics {

class EntityComponentSubSystem;

class EntityComponentSystem: public entt::registry {
public:
	EntityComponentSystem();

	void registerSubSystem(EntityComponentSubSystem* subSystem);
	void unregisterSubSystem(EntityComponentSubSystem* subSystem);

	void update();

private:
	std::vector<EntityComponentSubSystem*> _ecsSubSystems;
};

}
