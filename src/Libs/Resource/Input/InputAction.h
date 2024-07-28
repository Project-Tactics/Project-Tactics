#pragma once

#include "../Resource.h"

#include <Libs/Input/Click/Click.h>

namespace tactics::resource {

class InputAction : public Resource<InputAction> {
public:
	using Resource<InputAction>::Resource;
	static const ResourceType TYPE = ResourceType::InputAction;

	bool isTriggered(click::PlayerId playerId = 0) const;
	const click::ActionValue& getInputValue(click::PlayerId playerId = 0) const;

	click::ActionId actionId;
	std::vector<click::ActionState> states;
};

} // namespace tactics::resource
