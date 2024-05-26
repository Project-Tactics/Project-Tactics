#pragma once

#include <Libs/Resource/ResourceManager.h>
#include "Texture.h"

namespace tactics::resource {

class TextureManager: public TResourceManager<Texture> {
public:
	using TResourceManager<Texture>::TResourceManager;
	std::vector<ResourceId> load(sol::reference& luaDefinitionLoader) override;
};

}
