#pragma once

#include "../Resource.h"
#include "../Texture/Texture.h"

namespace tactics::resource {

class Sprite: public Resource<Sprite> {
public:
	using Resource<Sprite>::Resource;
	static const ResourceType type = ResourceType::Sprite;

	std::shared_ptr<Texture> texture;
};

}
