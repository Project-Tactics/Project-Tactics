#pragma once

#include "../Resource.h"

#include <Libs/Input/Click/Click.h>

namespace tactics::resource {

class InputMap : public Resource<InputMap> {
public:
	using Resource<InputMap>::Resource;
	static const ResourceType TYPE = ResourceType::InputMap;

	click::MapId mapId;
};

} // namespace tactics::resource
