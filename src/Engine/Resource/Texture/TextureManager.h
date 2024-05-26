#pragma once

#include <Libs/Resource/ResourceManager.h>
#include "Texture.h"

namespace tactics::resource {

class TextureManager: public TResourceManager<Texture> {
public:
	using TResourceManager<Texture>::TResourceManager;
	ResourceId load(const nlohmann::json& descriptor) override;

private:
	std::unique_ptr<Texture> _loadTexture(const std::string& name, const std::string& filename, bool useTransparency);
};

}
