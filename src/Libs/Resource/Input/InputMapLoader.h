#pragma once

#include "../ResourceLoader.h"
#include "InputMap.h"

namespace tactics::resource {

struct InputMapDescriptor {
	unsigned int type;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(InputMapDescriptor, type)
};

class InputMapLoader : public ResourceLoader {
public:
	using ResourceLoader::ResourceLoader;
	[[nodiscard]] std::shared_ptr<InputMap> load(const InputMapDescriptor& descriptor);
};

} // namespace tactics::resource
