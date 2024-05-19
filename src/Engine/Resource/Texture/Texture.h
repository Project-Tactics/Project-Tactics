#pragma once

#include "../Resource.h"

namespace tactics {

struct TextureInfo {
	int width;
	int height;
	int channelsCount;
};

class Texture: public Resource<Texture> {
public:
	static const ResourceType TYPE = ResourceType::Texture;
	TextureInfo info;
	unsigned int rendererId;
};

}
