#include "InputMapLoader.h"

namespace tactics::resource {

std::shared_ptr<InputMap> InputMapLoader::load(const InputMapDescriptor& /*descriptor*/) {
	auto inputMap = std::make_shared<InputMap>();
	return inputMap;
}

} // namespace tactics::resource
