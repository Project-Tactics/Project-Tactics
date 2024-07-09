#pragma once

#include "../ResourceLoader.h"
#include "InputAction.h"

namespace tactics::resource {

struct InputActionDescriptor {
	click::ActionType type;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(InputActionDescriptor, type)
};

class InputActionLoader : public ResourceLoader {
public:
	using ResourceLoader::ResourceLoader;
	[[nodiscard]] std::shared_ptr<InputAction> load(const InputActionDescriptor& descriptor);
};

} // namespace tactics::resource
