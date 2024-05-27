#pragma once

#include <Engine/Resource/Texture/Texture.h>
#include <Engine/Resource/Shader/Shader.h>

namespace tactics::components {

struct Material {
	std::shared_ptr<resource::Texture> texture;
	std::shared_ptr<resource::Shader> shader;
};

}
