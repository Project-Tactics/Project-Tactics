#include "MaterialLoader.h"

namespace tactics::resource {

std::shared_ptr<Material> MaterialLoader::load(const MaterialDescriptor& descriptor) {
	auto material = std::make_shared<Material>();

	auto shader = _getResource<Shader>(descriptor.shader);
	material->shader = shader;

	material->set(descriptor.uniforms.ints);
	material->set(descriptor.uniforms.floats);
	material->set(descriptor.uniforms.vectors2);
	material->set(descriptor.uniforms.vectors3);
	material->set(descriptor.uniforms.vectors4);
	material->set(descriptor.uniforms.matrices);

	for (const auto& [name, value] : descriptor.uniforms.textures) {
		auto texture = _getResource<Texture>(value);
		material->set(name, texture);
	}

	return material;
}

}
