#include "InputActionLoader.h"

namespace tactics::resource {

std::shared_ptr<InputAction> InputActionLoader::load(const InputActionDescriptor& descriptor) {
	auto inputAction = std::make_shared<InputAction>();
	// TODO(Gerark) This class is accessing directly to the click input library functions but we should never do that
	// We should use the InputSystem instead. Probably the Loader classes should get access to the service
	// locator to get the services they need
	inputAction->actionId = click::createAction(descriptor.type, descriptor.normalized);
	inputAction->states.resize(click::players());
	return inputAction;
}

} // namespace tactics::resource
