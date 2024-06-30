#include "MeshComponent.h"

#include <Libs/Ecs/Entity.h>
#include <Libs/Resource/ResourceProvider.h>
#include <Libs/Utility/Uniforms/UniformsDescriptor.h>

namespace tactics::component {

struct MeshDescriptor {
	std::string mesh;

	struct Material {
		std::string material;
		UniformsDescriptor uniforms;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(MeshDescriptor::Material, material, uniforms);
	};

	std::vector<Material> materials;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(MeshDescriptor, mesh, materials);
};

void Mesh::deserialize(const resource::ResourceProvider* resourceProvider, const nlohmann::ordered_json& jsonData) {
	MeshDescriptor descriptor = jsonData;
	mesh = resourceProvider->getResource<resource::Mesh>(HashId(descriptor.mesh));

	auto subMeshCount = mesh->subMeshes.size();
	if (subMeshCount != descriptor.materials.size()) {
		throw TACTICS_EXCEPTION("Number of materials does not match number of submeshes. SubMeshes: {} - Materials: {}",
								subMeshCount,
								descriptor.materials.size());
	}

	for (auto& material : descriptor.materials) {
		auto materialResource = resourceProvider->getResource<resource::Material>(HashId(material.material));
		auto materialInstance = resource::Material::createInstance(materialResource);
		UniformsDescriptor::fillUniformsInstance(material.uniforms, *materialInstance, *resourceProvider);
		materials.push_back(materialInstance);
	}
}

Mesh Mesh::clone() {
	Mesh clone;
	clone.mesh = mesh;
	for (auto& materialInstance : materials) { clone.materials.push_back(materialInstance->clone()); }
	return clone;
}

void Mesh::defineReflection() { componentReflection<Mesh>("mesh"); }

} // namespace tactics::component
