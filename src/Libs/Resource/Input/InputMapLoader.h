#pragma once

#include "../ResourceLoader.h"
#include "InputMap.h"

#include <Libs/Input/InputSerialization.h>
#include <Libs/Utility/Json/JsonSerialization.h>

namespace tactics::resource {

struct InputMapBindingGestureDescriptor {
	click::Gesture gesture;
	std::vector<click::Condition> conditions;
	std::vector<click::Modifier> modifiers;

	JSON_SERIALIZE(InputMapBindingGestureDescriptor, gesture, conditions, modifiers);
};

struct InputMapBindingDescriptor {
	HashId action;
	std::vector<InputMapBindingGestureDescriptor> gestures;

	JSON_SERIALIZE(InputMapBindingDescriptor, action, gestures);
};

struct InputMapDescriptor {
	unsigned int player;
	std::vector<InputMapBindingDescriptor> bindings;

	JSON_SERIALIZE(InputMapDescriptor, player, bindings);
};

class InputMapLoader : public ResourceLoader {
public:
	using ResourceLoader::ResourceLoader;
	[[nodiscard]] std::shared_ptr<InputMap> load(const InputMapDescriptor& descriptor);
};

} // namespace tactics::resource
