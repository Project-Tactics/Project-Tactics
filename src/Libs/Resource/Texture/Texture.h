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
	using Resource<Texture>::Resource;
	~Texture();
	void bind(unsigned int slot) const;
	void unbind() const;

	static std::shared_ptr<Texture> createNullTexture();

	TextureInfo info;
	unsigned int rendererId;
};

}
