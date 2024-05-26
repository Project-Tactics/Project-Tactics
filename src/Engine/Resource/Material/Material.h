#pragma once

#include <Libs/Resource/Resource.h>
#include <Libs/Utility/Color.h>

#include <glm/glm.hpp>

namespace tactics::resource {

class Material: public Resource<Material> {
public:
	static const ResourceType TYPE = ResourceType::Material;
	unsigned int shaderRendererId;
	unsigned int textureRendererId;
	glm::vec4 color = Color::white;
};

}
