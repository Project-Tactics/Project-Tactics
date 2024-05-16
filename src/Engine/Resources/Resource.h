#pragma once

#include <stdint.h>

enum class ResourceType {
	Shader
};

using ResourceId = uint64_t;

class BaseResource {
public:
	ResourceId id;
	ResourceType type;
};

template<typename TResource>
class Resource: public BaseResource {
};
