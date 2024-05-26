#include "Resource.h"

#include <Libs/Utility/UUID.h>
#include <Libs/Utility/Exception.h>

namespace tactics::resource {

ResourceType ResourceTypeSerialization::toEnum(std::string_view strValue) {
	if (strValue == "shader") {
		return ResourceType::Shader;
	} else if (strValue == "texture") {
		return ResourceType::Texture;
	} else if (strValue == "ini") {
		return ResourceType::IniFile;
	} else if (strValue == "mesh") {
		return ResourceType::Mesh;
	} else if (strValue == "material") {
		return ResourceType::Material;
	}

	return ResourceType::Unkwown;
}

ResourceType ResourceTypeSerialization::fromGroupTypeToEnum(std::string_view strValue) {
	if (strValue == "shaders") {
		return ResourceType::Shader;
	} else if (strValue == "textures") {
		return ResourceType::Texture;
	} else if (strValue == "inis") {
		return ResourceType::IniFile;
	} else if (strValue == "meshes") {
		return ResourceType::Mesh;
	} else if (strValue == "materials") {
		return ResourceType::Material;
	}

	return ResourceType::Unkwown;
}

std::string ResourceTypeSerialization::toString(ResourceType resourceType) {
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

BaseResource::BaseResource(std::string_view name, ResourceType type) {
	this->id = generateUUID();
	this->name = name;
	this->type = type;
}

}
