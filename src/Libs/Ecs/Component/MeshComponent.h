#pragma once

#include <Libs/Resource/Mesh/Mesh.h>
#include <Libs/Resource/Material/Material.h>
#include <Libs/Utility/Reflection.h>

namespace tactics::component {

struct Mesh {
	std::shared_ptr<resource::Mesh> mesh;
	std::vector<std::shared_ptr<resource::Material::Instance>> materials;

	static void defineReflection();
	void deserialize(const resource::ResourceProvider* resourceProvider, const nlohmann::ordered_json& jsonData);
	Mesh clone();
};

}
