#pragma once

#include <Libs/Resource/ResourceManager.h>
#include "Mesh.h"

namespace tactics {

class MeshManager: public TResourceManager<Mesh> {
public:
	using TResourceManager<Mesh>::TResourceManager;
	std::vector<ResourceId> load(sol::reference& luaDefinitionLoader) override;
	void unload(ResourceId resourceId) override;
	void unload(std::vector<ResourceId> resourceIds) override;
};

}
