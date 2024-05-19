#pragma once

#include <Libs/Resource/ResourceManager.h>
#include "Shader.h"

namespace tactics {

class ShaderManager: public TResourceManager<Shader> {
public:
	using TResourceManager<Shader>::TResourceManager;
	std::vector<ResourceId> load(sol::reference& luaDefinitionLoader) override;
	void unload(ResourceId resourceId) override;
	void unload(std::vector<ResourceId> resourceIds) override;
};

}
