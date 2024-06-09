#pragma once

#include <Libs/Resource/Mesh/Mesh.h>
#include <Libs/Resource/Material/Material.h>

namespace tactics::component {

struct Mesh {
	std::shared_ptr<resource::Mesh> mesh;
	std::vector<std::shared_ptr<resource::Material::Instance>> materials;

	static void defineReflection();
	Mesh clone();
};

}
