#pragma once

#include <Libs/Resource/Resource.h>

namespace tactics::resource {

struct TextureInfo {
	int width;
	int height;
	int channelsCount;
	bool useTransparency;
};

class Texture: public Resource<Texture> {
public:
	static const ResourceType TYPE = ResourceType::Texture;
	~Texture();
	TextureInfo info;
	unsigned int rendererId;
};

}
