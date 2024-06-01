#pragma once

#include <Libs/Resource/ResourceLoader.h>
#include "Texture.h"

namespace tactics::resource {

struct TextureDescriptor {
	std::string path;
	bool useTransparency = false;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(TextureDescriptor, path, useTransparency);
};

class TextureLoader: public ResourceLoader {
public:
	using ResourceLoader::ResourceLoader;
	std::shared_ptr<Texture> load(const std::string& name, const TextureDescriptor& descriptor);

private:
	std::shared_ptr<Texture> _loadTexture(const std::string& name, const std::string& filename, bool useTransparency);
};

}
