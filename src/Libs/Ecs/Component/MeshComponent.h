#pragma once

#include <Libs/Resource/Material/Material.h>
#include <Libs/Resource/Mesh/Mesh.h>
#include <Libs/Utility/Reflection.h>

namespace tactics::component {

struct Mesh {
	std::shared_ptr<resource::Mesh> mesh;
	resource::MaterialInstances materials;

	Mesh clone();

	COMPONENT(Mesh, mesh, materials);
};

} // namespace tactics::component
