#include "InputActionLoader.h"

namespace tactics::resource {

std::shared_ptr<InputAction> InputActionLoader::load(const InputActionDescriptor& descriptor) {
	auto inputAction = std::make_shared<InputAction>();
	inputAction->actionId = click::createAction(descriptor.type);
	return inputAction;
}

} // namespace tactics::resource
