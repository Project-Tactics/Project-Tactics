#pragma once

#include <Libs/Utility/HashId.h>
#include <Libs/Utility/Json/JsonSerialization.h>
#include <Libs/Utility/String/String.h>

#include <stdint.h>

namespace tactics::resource {

enum class ResourceType {
	DataSet,
	IniFile,
	InputAction,
	InputMap,
	Material,
	Mesh,
	ParticleEffect,
	Prefab,
	Shader,
	SpriteSheet,
	Texture,
	Unkwown
};

} // namespace tactics::resource

FORMAT_STR(tactics::resource::ResourceType);

namespace tactics::resource {

const std::array resourceTypeLoadingOrder = {ResourceType::IniFile,
											 ResourceType::InputAction,
											 ResourceType::InputMap,
											 ResourceType::Shader,
											 ResourceType::Texture,
											 ResourceType::Material,
											 ResourceType::Mesh,
											 ResourceType::SpriteSheet,
											 ResourceType::ParticleEffect,
											 ResourceType::Prefab,
											 ResourceType::DataSet};

using ResourceId = uint64_t;

class BaseResource {
public:
	BaseResource(HashId name, ResourceType type);
	BaseResource(BaseResource&&) = delete;
	BaseResource(BaseResource&) = delete;
	BaseResource& operator=(BaseResource&&) = delete;
	virtual ~BaseResource() = default;

	ResourceId id;
	HashId name;
	ResourceType type;
};

template<typename TResource> class Resource : public BaseResource {
public:
	using Ptr = std::shared_ptr<TResource>;

	explicit Resource(HashId name) : BaseResource(name, TResource::TYPE) {}

	Resource() : BaseResource(""_id, TResource::TYPE) {}
};

/*
 * Common file descriptor used to extract info from json objects when the resource has just a name and a path for the
 * file
 */
struct FileDescriptor {
	std::string path;

	JSON_SERIALIZE(FileDescriptor, path);
};

template<typename T>
concept IsResource = std::derived_from<T, Resource<T>> && !std::same_as<T, Resource<T>>;

} // namespace tactics::resource
