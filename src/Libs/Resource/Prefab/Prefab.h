#pragma once

#include "../Resource.h"

#include <Libs/Ecs/Entity.h>

namespace tactics::resource {

class Prefab : public Resource<Prefab> {
public:
	static const ResourceType TYPE = ResourceType::Prefab;
	using Resource<Prefab>::Resource;
	~Prefab();
	Entity entity;
};

} // namespace tactics::resource
