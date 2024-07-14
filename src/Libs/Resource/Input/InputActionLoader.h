#pragma once

#include "../ResourceLoader.h"
#include "InputAction.h"

#include <Libs/Input/InputSerialization.h>

namespace tactics::resource {

struct InputActionDescriptor {
	click::ActionType type = click::ActionType::Scalar;
	bool normalized = false;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(InputActionDescriptor, type, normalized)
};

class InputActionLoader : public ResourceLoader {
public:
	using ResourceLoader::ResourceLoader;
	[[nodiscard]] std::shared_ptr<InputAction> load(const InputActionDescriptor& descriptor);
};

} // namespace tactics::resource
