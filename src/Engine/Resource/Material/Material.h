#pragma once

#include "Uniforms.h"

#include <Engine/Resource/Shader/Shader.h>
#include <Engine/Resource/Texture/Texture.h>

#include <Libs/Resource/Resource.h>
#include <Libs/Utility/Color.h>
#include <Libs/Utility/TransparentTypes.h>

#include <glm/glm.hpp>

namespace tactics::resource {

class Material: public Resource<Material>, public Uniforms {
public:
	static const ResourceType TYPE = ResourceType::Material;
	using Resource<Material>::Resource;

	class Instance: public Uniforms {
	public:
		explicit Instance(std::shared_ptr<Material> parent);
		void updateShaderUniforms();

		std::shared_ptr<Material> parent;
	};

	static std::shared_ptr<Material::Instance> createInstance(std::shared_ptr<Material> parentMaterial);

	std::shared_ptr<Shader> shader;
};

}
