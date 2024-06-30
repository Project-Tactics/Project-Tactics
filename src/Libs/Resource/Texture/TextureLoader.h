#pragma once

#include "Texture.h"

#include <Libs/Resource/ResourceLoader.h>

namespace tactics::resource {

struct TextureDescriptor {
	std::string path;
	bool useTransparency = false;
	TextureFilter filter = TextureFilter::Linear;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(TextureDescriptor, path, useTransparency, filter);
};

class TextureLoader : public ResourceLoader {
public:
	using ResourceLoader::ResourceLoader;
	[[nodiscard]] std::shared_ptr<Texture> load(const HashId& name, const TextureDescriptor& descriptor);

private:
	std::shared_ptr<Texture>
	_loadTexture(const HashId& name, const std::string& filename, const TextureDescriptor& descriptor);
};

} // namespace tactics::resource
