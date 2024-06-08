#include "MeshComponent.h"

#include <Libs/Ecs/Entity.h>
#include <Libs/Resource/ResourceSystem.h>
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

void meshDeserializer(Mesh& mesh, entt::registry* registry, const nlohmann::ordered_json& jsonData) {
	MeshDescriptor descriptor = jsonData;
	mesh.mesh = getService<resource::ResourceSystem>(*registry).getResource<resource::Mesh>(descriptor.mesh);

	if (mesh.mesh->subMeshes.size() != descriptor.materials.size()) {
		throw TACTICS_EXCEPTION("Number of materials does not match number of submeshes");
	}

	for (auto& material : descriptor.materials) {
		auto materialResource = getService<resource::ResourceSystem>(*registry).getResource<resource::Material>(material.material);
		auto materialInstance = resource::Material::createInstance(materialResource);
		UniformsDescriptor::fillUniformsInstance(material.uniforms, *materialInstance, getService<resource::ResourceSystem>(*registry));
		mesh.materials.push_back(materialInstance);
	}
}

void Mesh::defineReflection() {
	entt::meta<Mesh>()
		.type(hash("mesh"))
		.func<&Entity::explicitAddComponent<Mesh>, entt::as_ref_t>(hash("emplace"))
		.func<&meshDeserializer>(hash("deserializer"));
}

}
