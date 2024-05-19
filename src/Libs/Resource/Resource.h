#pragma once

#include <stdint.h>
#include <string>

namespace tactics {

enum class ResourceType: uint8_t {
	Unkwown,
	Shader,
	Texture
};

// TODO(Gerark) we should take a look at magic_enum lib or something else to automate this
class ResourceTypeSerialization {
public:
	static ResourceType toEnum(std::string_view strValue);
	static std::string toString(ResourceType resourceType);
	static ResourceType fromGroupTypeToEnum(std::string_view strValue);
};

using ResourceId = uint64_t;

class BaseResource {
public:
	BaseResource(std::string_view name, ResourceType type);

	ResourceId id;
	std::string name;
	ResourceType type;
};

template<typename TResource>
class Resource: public BaseResource {
public:
	Resource(std::string_view name): BaseResource(name, TResource::TYPE) {}
};

}
