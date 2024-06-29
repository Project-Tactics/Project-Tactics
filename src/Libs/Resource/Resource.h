#pragma once

#include <Libs/Utility/String.h>
#include <Libs/Utility/HashString.h>

#include <nlohmann/json.hpp>

#include <stdint.h>
#include <string>

namespace tactics::resource {

enum class ResourceType {
	IniFile,
	Material,
	Mesh,
	Prefab,
	Shader,
	SpriteSheet,
	Texture,
	Unkwown
};

const std::array<ResourceType, 7> resourceTypeLoadingOrder = {
	ResourceType::IniFile,
	ResourceType::Shader,
	ResourceType::Texture,
	ResourceType::Material,
	ResourceType::Mesh,
	ResourceType::SpriteSheet,
	ResourceType::Prefab
};

using ResourceId = uint64_t;

class BaseResource {
public:
	BaseResource(hash_string name, ResourceType type);
	BaseResource(BaseResource&&) = delete;
	BaseResource(BaseResource&) = delete;
	BaseResource& operator=(BaseResource&&) = delete;
	virtual ~BaseResource() = default;

	ResourceId id;
	hash_string name;
	ResourceType type;
};

template<typename TResource>
class Resource: public BaseResource {
public:
	explicit Resource(hash_string name): BaseResource(name, TResource::TYPE) {}
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

}

template <>
struct fmt::formatter<tactics::resource::ResourceType > {
public:
	constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
	template <typename Context>
	constexpr auto format(tactics::resource::ResourceType const& obj, Context& ctx) const {
		return fmt::format_to(ctx.out(), "{}", tactics::Str<tactics::resource::ResourceType>::to(obj));
	}
};
