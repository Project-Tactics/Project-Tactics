#pragma once

#include <stdint.h>
#include <string>

namespace tactics {

enum class ResourceType {
	Shader
};

using ResourceId = uint64_t;

class BaseResource {
public:
	ResourceId id;
	std::string name;
	ResourceType type;
};

template<typename TResource>
class Resource: public BaseResource {
};

}
