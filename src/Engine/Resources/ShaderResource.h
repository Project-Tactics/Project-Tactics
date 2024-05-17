#pragma once

#include "Resource.h"
#include "ShaderLoader.h"

namespace tactics {

class ShaderResource: public Resource<ShaderResource> {
public:
	ShaderId shaderId;
};

}
