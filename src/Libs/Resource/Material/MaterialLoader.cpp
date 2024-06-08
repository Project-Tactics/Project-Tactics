#include "MaterialLoader.h"

#include <Libs/Utility/Exception.h>

namespace tactics::resource {

std::shared_ptr<Material> MaterialLoader::load(const MaterialDescriptor& descriptor) {
	auto material = std::make_shared<Material>();

	if (descriptor.shader.empty()) {
		throw TACTICS_EXCEPTION("MaterialDescriptor must have a valid shader.");
	}

	auto shader = _getResource<Shader>(descriptor.shader);
	material->shader = shader;

	UniformsDescriptor::fillUniformsInstance(descriptor.uniforms, *material, _getResourceProvider());

	return material;
}

}
