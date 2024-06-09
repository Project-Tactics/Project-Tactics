#pragma once

#include "Uniforms.h"


#include <Libs/Resource/Resource.h>
#include <Libs/Resource/Shader/Shader.h>
#include <Libs/Resource/Texture/Texture.h>
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
		[[nodiscard]] std::shared_ptr<Instance> clone();

		std::shared_ptr<Material> parent;
	};

	[[nodiscard]] static std::shared_ptr<Material::Instance> createInstance(std::shared_ptr<Material> parentMaterial);

	bool hasAlphaBlend{};
	std::shared_ptr<Shader> shader;
};

}
