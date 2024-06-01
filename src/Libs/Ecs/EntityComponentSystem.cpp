#include "EntityComponentSystem.h"

#include "SubSystem/EntityComponentSubSystem.h"

#include <Libs/Utility/Exception.h>

#include <ranges>

namespace tactics {

EntityComponentSystem::EntityComponentSystem() {
}

void EntityComponentSystem::registerSubSystem(EntityComponentSubSystem* subSystem) {
	if (std::ranges::find(_ecsSubSystems, subSystem) != _ecsSubSystems.end()) {
		throw Exception("SubSystem already registered");
	}
	_ecsSubSystems.push_back(subSystem);
}

void EntityComponentSystem::update() {
	for (auto& subSystem : _ecsSubSystems) {
		subSystem->update();
	}
}

void EntityComponentSystem::unregisterSubSystem(EntityComponentSubSystem* subSystem) {
	if (auto itr = std::ranges::find(_ecsSubSystems, subSystem); itr != _ecsSubSystems.end()) {
		_ecsSubSystems.erase(itr);
	} else {
		throw Exception("SubSystem can't be removed cause it's not registered");
	}
}

}
