#pragma once

#include "../Resource.h"
#include "InputAction.h"

#include <Libs/Input/Click/Click.h>

namespace tactics::resource {

struct InputBinding {
	std::shared_ptr<InputAction> action;
	click::Gesture gesture;
	std::vector<click::Condition> triggers;
	std::vector<click::Modifier> modifiers;
};

class InputMap : public Resource<InputMap> {
public:
	using Resource<InputMap>::Resource;
	static const ResourceType TYPE = ResourceType::InputMap;

	std::vector<InputBinding> bindings;
	click::MapId mapId{};
};

} // namespace tactics::resource
