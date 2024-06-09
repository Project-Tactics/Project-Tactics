#include "MeshComponent.h"

#include <Libs/Ecs/Entity.h>
#include <Libs/Resource/ResourceProvider.h>
#include <Libs/Utility/Uniforms/UniformsDescriptor.h>
#include <Libs/Utility/Reflection.h>

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

void deserializeMesh(Mesh& mesh, const resource::ResourceProvider* resourceProvider, const nlohmann::ordered_json& jsonData) {
	MeshDescriptor descriptor = jsonData;
	mesh.mesh = resourceProvider->getResource<resource::Mesh>(descriptor.mesh);

	if (mesh.mesh->subMeshes.size() != descriptor.materials.size()) {
		throw TACTICS_EXCEPTION("Number of materials does not match number of submeshes");
	}

	for (auto& material : descriptor.materials) {
		auto materialResource = resourceProvider->getResource<resource::Material>(material.material);
		auto materialInstance = resource::Material::createInstance(materialResource);
		UniformsDescriptor::fillUniformsInstance(material.uniforms, *materialInstance, *resourceProvider);
		mesh.materials.push_back(materialInstance);
	}
}

Mesh Mesh::clone() {
	Mesh clone;
	clone.mesh = mesh;
	for (auto& materialInstance : materials) {
		clone.materials.push_back(materialInstance->clone());
	}
	return clone;
}

void Mesh::defineReflection() {
	componentReflection<Mesh>("mesh")
		.func<&deserializeMesh>(hash("deserializer"));
}

}
