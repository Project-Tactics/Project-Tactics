#pragma once

#include <Engine/Resource/Texture/Texture.h>
#include <Engine/Resource/Shader/Shader.h>

namespace tactics::components {

struct Material {
	resource::Texture* texture;
	resource::Shader* shader;
};

}
