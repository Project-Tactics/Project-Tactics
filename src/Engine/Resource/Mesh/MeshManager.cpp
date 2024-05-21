#include "MeshManager.h"

#include "MeshLoader.h"

#include <Libs/Resource/ResourcePathHelper.h>
#include <Libs/Scripting/ScriptingHelper.h>

namespace tactics {

std::vector<ResourceId> MeshManager::load(sol::reference& luaDefinitionLoader) {
	sol::environment resourcePackEnv = _luaState["__resourceEnvTable"];

	std::vector<ResourceId> resources;

	// This way of defining a mesh from lua in an inline style through strings is not that efficient but it's mostly used for simple meshes
	resourcePackEnv.set_function("mesh", [this, &resources] (std::string_view name, std::string strVertices, std::string strIndices) {
		auto mesh = std::make_unique<Mesh>(name);
		mesh->vertexBuffer = std::make_unique<VertexBuffer>();
		mesh->vertexBuffer->setData(MeshLoader::parseVertices(strVertices));
		mesh->vertexBuffer->unbind();
		mesh->indexBuffer = std::make_unique<IndexBuffer>();
		mesh->indexBuffer->setData(MeshLoader::parseIndices(strIndices));
		mesh->indexBuffer->unbind();

		resources.push_back(mesh->id);
		_registerResource(std::move(mesh));
	});

	ScriptingHelper::executeFunction(_luaState, luaDefinitionLoader);

	resourcePackEnv["mesh"] = sol::nil;
	return resources;
}

void MeshManager::unload(ResourceId resourceId) {
	auto resource = getResource(resourceId);
	_removeResource(resource);
}

void MeshManager::unload(std::vector<ResourceId> resourceIds) {
	for (auto& resourceId : resourceIds) {
		unload(resourceId);
	}

}

}
