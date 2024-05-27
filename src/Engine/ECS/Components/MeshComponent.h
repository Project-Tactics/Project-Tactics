#pragma once

#include <Engine/Resource/Mesh/Mesh.h>
#include <Engine/Resource/Material/Material.h>

namespace tactics::components {

struct Mesh {
	std::shared_ptr<resource::Mesh> mesh;
	std::shared_ptr<resource::Material::Instance> material;
};

}
