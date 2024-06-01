#pragma once

#include "Material.h"

#include <Libs/Resource/ResourceLoader.h>
#include <Libs/Utility/Json/MathJsonSerializer.h>

namespace tactics::resource {

struct MaterialDescriptorUniforms {
	std::unordered_map<std::string, std::string> textures;
	std::unordered_map<std::string, int> ints;
	std::unordered_map<std::string, float> floats;
	std::unordered_map<std::string, glm::mat4> matrices;
	std::unordered_map<std::string, glm::vec2> vectors2;
	std::unordered_map<std::string, glm::vec3> vectors3;
	std::unordered_map<std::string, glm::vec4> vectors4;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(MaterialDescriptorUniforms, textures, vectors4, vectors3, vectors2, floats, ints);
};

struct MaterialDescriptor {
	std::string shader;
	MaterialDescriptorUniforms uniforms;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(MaterialDescriptor, shader, uniforms);
};

class MaterialLoader: public ResourceLoader {
public:
	using ResourceLoader::ResourceLoader;
	std::shared_ptr<Material> load(const MaterialDescriptor& descriptor);
};

}
