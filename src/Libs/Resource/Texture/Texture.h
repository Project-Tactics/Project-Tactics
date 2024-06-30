#pragma once

#include <Libs/Resource/Resource.h>

namespace tactics::resource {

enum class TextureFilter {
	Linear,
	Nearest
};

NLOHMANN_JSON_SERIALIZE_ENUM(TextureFilter,
							 {
								 {TextureFilter::Linear, "linear"},
								 {TextureFilter::Nearest, "nearest"},
							 })

struct TextureInfo {
	int width;
	int height;
	int channelsCount;
	bool useTransparency;
	TextureFilter filter;
};

class Texture : public Resource<Texture> {
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

} // namespace tactics::resource
