#pragma once

#include "Material.h"

#include <Libs/Resource/ResourceLoader.h>
#include <Libs/Utility/Uniforms/UniformsDescriptor.h>

namespace tactics::resource {

struct MaterialDescriptor {
	std::string shader;
	bool alphaBlended{};
	UniformsDescriptor uniforms;

	JSON_SERIALIZE(MaterialDescriptor, shader, alphaBlended, uniforms);
};

class MaterialLoader : public ResourceLoader {
public:
	using ResourceLoader::ResourceLoader;
	[[nodiscard]] std::shared_ptr<Material> load(const MaterialDescriptor& descriptor);
};

} // namespace tactics::resource
