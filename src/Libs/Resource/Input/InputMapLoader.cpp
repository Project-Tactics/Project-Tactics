#include "InputMapLoader.h"

namespace tactics::resource {

std::shared_ptr<InputMap> InputMapLoader::load(const InputMapDescriptor& descriptor) {
	auto inputMap = std::make_shared<InputMap>();

	for (const auto& binding : descriptor.bindings) {
		inputMap->bindings.emplace_back(_getResource<InputAction>(binding.action),
										binding.gesture,
										binding.triggers,
										binding.modifiers);
	}

	return inputMap;
}

} // namespace tactics::resource
