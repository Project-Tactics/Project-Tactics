#include "ResourceSerialization.h"

#include <Libs/Utility/Exception.h>
#include <Libs/Utility/Uniforms/UniformsDescriptor.h>

namespace tactics::resource {

ResourceProvider* _resourceProvider = nullptr;

void setResourceProviderForSerialization(tactics::resource::ResourceProvider* provider) {
	_resourceProvider = provider;
}

struct MaterialInstanceDescriptor {
	tactics::resource::Material::Ptr material;
	tactics::UniformsDescriptor uniforms;

	JSON_SERIALIZE(MaterialInstanceDescriptor, material, uniforms);
};

} // namespace tactics::resource

namespace nlohmann {

tactics::resource::ResourceProvider* resource_serializer_base::_getResourceProvider() {
	using namespace tactics::resource;
	if (_resourceProvider == nullptr) {
		throw TACTICS_EXCEPTION("Resource provider for serialization is not set");
	}
	return _resourceProvider;
}

void adl_serializer<tactics::resource::MaterialInstances>::to_json(json&, const tactics::resource::MaterialInstances&) {
	throw TACTICS_EXCEPTION("Serializing resources is not yet implemented");
}

void adl_serializer<tactics::resource::MaterialInstances>::from_json(
	const json& json,
	tactics::resource::MaterialInstances& materialInstances) {
	using namespace tactics::resource;
	using namespace tactics;
	std::vector<MaterialInstanceDescriptor> descriptors = json;
	for (auto& descriptor : descriptors) {
		auto materialInstance = Material::createInstance(descriptor.material);
		tactics::UniformsDescriptor::fillUniformsInstance(descriptor.uniforms, *materialInstance, *_resourceProvider);
		materialInstances.push_back(materialInstance);
	}
}

} // namespace nlohmann
