#pragma once

#include "Resource.h"

#include <string>

class ShaderResource: public Resource<ShaderResource> {
public:
	std::string name;
	std::string vertexShader;
	std::string fragmentShader;
};
