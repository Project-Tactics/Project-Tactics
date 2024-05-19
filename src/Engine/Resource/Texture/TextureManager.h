#pragma once

#include "../ResourceManager.h"
#include "Texture.h"

namespace tactics {

class TextureManager: public TResourceManager<Texture> {
public:
	using TResourceManager<Texture>::TResourceManager;
	std::vector<ResourceId> load(sol::reference& luaDefinitionLoader) override;
	void unload(ResourceId resourceId) override;
	void unload(std::vector<ResourceId> resourceIds) override;

private:
	ResourceId _createTexture(std::string_view name, std::string_view filename);
};

}
