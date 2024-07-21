#pragma once

#include "../ResourceLoader.h"
#include "InputMap.h"

#include <Libs/Input/InputSerialization.h>
#include <Libs/Utility/Json/JsonSerialization.h>

namespace tactics::resource {

struct InputMapBindingGestureDescriptor {
	click::Gesture gesture;
	std::vector<click::Trigger> triggers;
	std::vector<click::Modifier> modifiers;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(InputMapBindingGestureDescriptor, gesture, triggers, modifiers);
};

struct InputMapBindingDescriptor {
	HashId action;
	std::vector<InputMapBindingGestureDescriptor> gestures;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(InputMapBindingDescriptor, action, gestures);
};

struct InputMapDescriptor {
	unsigned int player;
	std::vector<InputMapBindingDescriptor> bindings;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(InputMapDescriptor, player, bindings);
};

class InputMapLoader : public ResourceLoader {
public:
	using ResourceLoader::ResourceLoader;
	[[nodiscard]] std::shared_ptr<InputMap> load(const InputMapDescriptor& descriptor);
};

} // namespace tactics::resource
