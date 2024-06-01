#pragma once

#include "EntityComponentSubSystem.h"

namespace tactics {

class CameraSubSystem: public EntityComponentSubSystem {
public:
	using EntityComponentSubSystem::EntityComponentSubSystem;
	void update() override;
};

}
