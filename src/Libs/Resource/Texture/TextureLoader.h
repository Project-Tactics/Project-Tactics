#pragma once

#include <Libs/Resource/ResourceLoader.h>
#include "Texture.h"

namespace tactics::resource {

struct TextureDescriptor {
	std::string path;
	bool useTransparency = false;
	TextureFilter filter = TextureFilter::Linear;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(TextureDescriptor, path, useTransparency, filter);
};

class TextureLoader: public ResourceLoader {
public:
	using ResourceLoader::ResourceLoader;
	[[nodiscard]] std::shared_ptr<Texture> load(const std::string& name, const TextureDescriptor& descriptor);

private:
	std::shared_ptr<Texture> _loadTexture(const std::string& name, const std::string& filename, const TextureDescriptor& descriptor);
};

}
