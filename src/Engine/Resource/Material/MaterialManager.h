#pragma once

#include "Material.h"

#include <Libs/Resource/ResourceManager.h>

namespace tactics::resource {

class MaterialManager: public TResourceManager<Material> {
public:
	using TResourceManager<Material>::TResourceManager;

	std::vector<ResourceId> load(sol::reference& luaDefinitionLoader) override;
};

}
