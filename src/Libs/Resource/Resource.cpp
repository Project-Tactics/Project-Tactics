#include "Resource.h"

#include <Libs/Utility/UUID.h>
#include <Libs/Utility/Exception.h>

namespace tactics::resource {

BaseResource::BaseResource(const std::string& name, ResourceType type) {
	this->id = generateUUID();
	this->name = name;
	this->type = type;
}

}

namespace tactics {

std::string Str<resource::ResourceType>::to(resource::ResourceType resourceType) {
	using namespace resource;

	switch (resourceType) {
	case ResourceType::Texture: {
		return "texture";
	}
	case ResourceType::Shader: {
		return "shader";
	}
	case ResourceType::IniFile: {
		return "ini";
	}
	case ResourceType::Mesh: {
		return "mesh";
	}
	case ResourceType::Material: {
		return "material";
	}
	}

	return "Unknown";
}

resource::ResourceType Str<resource::ResourceType>::from(std::string_view string) {
	using namespace resource;

	if (string == "shader") {
		return ResourceType::Shader;
	} else if (string == "texture") {
		return ResourceType::Texture;
	} else if (string == "ini") {
		return ResourceType::IniFile;
	} else if (string == "mesh") {
		return ResourceType::Mesh;
	} else if (string == "material") {
		return ResourceType::Material;
	}

	return ResourceType::Unkwown;
}

}
