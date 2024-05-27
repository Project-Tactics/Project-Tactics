#pragma once

#include <Engine/Resource/Mesh/Mesh.h>

namespace tactics::components {

struct Mesh {
	std::shared_ptr<resource::Mesh> mesh;
};

}
