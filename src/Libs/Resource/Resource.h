#pragma once

#include <Libs/Utility/String.h>

#include <nlohmann/json.hpp>

#include <stdint.h>
#include <string>

namespace tactics::resource {

enum class ResourceType: uint8_t {
	IniFile,
	Material,
	Mesh,
	Prefab,
	Shader,
	Texture,
	Unkwown
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
	Resource(): BaseResource("", TResource::TYPE) {}
};

/*
 * Common file descriptor used to extract info from json objects when the resource has just a name and a path for the file
*/
struct FileDescriptor {
	std::string path;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(FileDescriptor, path);
};

}

namespace tactics {

template<>
class Str<resource::ResourceType> {
public:
	static std::string to(resource::ResourceType resourceType);
	static resource::ResourceType from(std::string_view string);
};

namespace resource {
using ResourceTypeStr = Str<resource::ResourceType>;
}

}
