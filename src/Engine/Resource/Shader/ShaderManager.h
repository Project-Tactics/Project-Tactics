#pragma once

#include <Libs/Resource/ResourceManager.h>
#include "Shader.h"

namespace tactics::resource {

class ShaderManager: public TResourceManager<Shader> {
public:
	using TResourceManager<Shader>::TResourceManager;
	std::vector<ResourceId> load(sol::reference& luaDefinitionLoader) override;
};

}
