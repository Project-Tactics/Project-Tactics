#pragma once

#include <Libs/Resource/ResourceManager.h>
#include "Mesh.h"
#include "MeshLoader.h"

namespace tactics::resource {

class MeshManager: public TResourceManager<Mesh> {
public:
	using TResourceManager<Mesh>::TResourceManager;
	std::vector<ResourceId> load(sol::reference& luaDefinitionLoader) override;

private:
	MeshLoader _meshLoader;
};

}
