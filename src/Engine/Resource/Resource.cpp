#include "Resource.h"

#include <Libs/Utilities/UUID.h>
#include <Libs/Utilities/Exception.h>

namespace tactics {

ResourceType ResourceTypeSerialization::toEnum(std::string_view strValue) {
	if (strValue == "shader") {
		return ResourceType::Shader;
	} else if (strValue == "texture") {
		return ResourceType::Texture;
	}

	return ResourceType::Unkwown;
}

ResourceType ResourceTypeSerialization::fromGroupTypeToEnum(std::string_view strValue) {
	if (strValue == "shaders") {
		return ResourceType::Shader;
	} else if (strValue == "textures") {
		return ResourceType::Texture;
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
	}

	return "Unknown";
}

BaseResource::BaseResource(std::string_view name, ResourceType type) {
	this->id = generateUUID();
	this->name = name;
	this->type = type;
}

}
