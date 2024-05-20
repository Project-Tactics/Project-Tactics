#pragma once

#include <Libs/Resource/Resource.h>

namespace tactics {

class Mesh: public Resource<Mesh> {
public:
	static const ResourceType TYPE = ResourceType::Mesh;
};

}
