#pragma once

#include <stdint.h>
#include <string>

#include <nlohmann/json.hpp>

namespace tactics::resource {

enum class ResourceType: uint8_t {
	Unkwown,
	Shader,
	Texture,
	IniFile,
	Mesh,
	Material
};

// TODO(Gerark) we should take a look at magic_enum lib or something else to automate this
class ResourceTypeSerialization {
public:
	static ResourceType toEnum(std::string_view strValue);
	static std::string toString(ResourceType resourceType);
};

using ResourceId = uint64_t;

class BaseResource {
public:
	BaseResource(const std::string& name, ResourceType type);
	BaseResource(BaseResource&&) = delete;
	BaseResource(BaseResource&) = delete;
	BaseResource& operator=(BaseResource&&) = delete;
	virtual ~BaseResource() = default;

	ResourceId id;
	std::string name;
	ResourceType type;
};

template<typename TResource>
class Resource: public BaseResource {
public:
	explicit Resource(const std::string& name): BaseResource(name, TResource::TYPE) {}
};

/*
 * Common file descriptor used to extract info from json objects when the resource has just a name and a path for the file
*/
struct FileDescriptor {
	std::string name;
	std::string path;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(FileDescriptor, name, path);
};

}
