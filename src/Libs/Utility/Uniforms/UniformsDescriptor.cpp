#include "UniformsDescriptor.h"

#include <Libs/Resource/Material/Uniforms.h>
#include <Libs/Resource/Texture/Texture.h>
#include <Libs/Resource/ResourceProvider.h>

namespace tactics {

void UniformsDescriptor::fillUniformsInstance(const UniformsDescriptor& descriptor, resource::Uniforms& uniforms, const resource::ResourceProvider& resourceProvider) {
	fillUniformsInstanceWithBasicTypes(descriptor, uniforms);

	for (const auto& [name, value] : descriptor.textures) {
		auto texture = resourceProvider.getResource<resource::Texture>(value);
		uniforms.set(name, texture);
	}
}

void UniformsDescriptor::fillUniformsInstanceWithBasicTypes(const UniformsDescriptor& descriptor, resource::Uniforms& uniforms) {
	uniforms.set(descriptor.ints);
	uniforms.set(descriptor.floats);
	uniforms.set(descriptor.vectors2);
	uniforms.set(descriptor.vectors3);
	uniforms.set(descriptor.vectors4);
	uniforms.set(descriptor.matrices);
}

}
