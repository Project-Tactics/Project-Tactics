#include "MeshManager.h"

#include <Libs/Resource/ResourcePathHelper.h>
#include <Libs/Scripting/ScriptingHelper.h>

namespace tactics {

std::vector<ResourceId> MeshManager::load(sol::reference& luaDefinitionLoader) {
	sol::environment resourcePackEnv = _luaState["__resourceEnvTable"];

	std::vector<ResourceId> resources;

	resourcePackEnv.set_function("mesh", [this, &resources] (std::string_view name) {
		auto mesh = std::make_unique<Mesh>(name);
		resources.push_back(mesh->id);
		_registerResource(std::move(mesh));
	});

	resourcePackEnv.set_function("meshDef", [this, &resourcePackEnv] (std::string_view definitionFile) {
		auto path = _pathHelper.makeAbsolutePath(definitionFile);
		_luaState.script_file(path, resourcePackEnv);
	});

	ScriptingHelper::executeFunction(_luaState, luaDefinitionLoader);

	resourcePackEnv["mesh"] = sol::nil;
	resourcePackEnv["meshDef"] = sol::nil;
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
