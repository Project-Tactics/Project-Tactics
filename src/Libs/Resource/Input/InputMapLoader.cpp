#include "InputMapLoader.h"

namespace tactics::resource {

std::shared_ptr<InputMap> InputMapLoader::load(const InputMapDescriptor& descriptor) {
	auto inputMap = std::make_shared<InputMap>();

	for (const auto& binding : descriptor.bindings) {
		for (const auto& gesture : binding.gestures) {
			inputMap->bindings.emplace_back(_getResource<InputAction>(binding.action),
											gesture.gesture,
											gesture.triggers,
											gesture.modifiers);
		}
	}

	return inputMap;
}

} // namespace tactics::resource
