#pragma once

#include <Libs/Resource/Resource.h>

namespace tactics::resource {

class Shader: public Resource<Shader> {
public:
	static const ResourceType TYPE = ResourceType::Shader;
	using Resource<Shader>::Resource;
	~Shader();
	unsigned int rendererId;
};

}
