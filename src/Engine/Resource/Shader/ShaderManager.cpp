#include "ShaderManager.h"

#include "ShaderLoader.h"

#include <Libs/Resource/ResourcePathHelper.h>
#include <Libs/Scripting/ScriptingHelper.h>

namespace tactics {

std::vector<ResourceId> ShaderManager::load(sol::reference& luaDefinitionLoader) {
	sol::environment resourcePackEnv = _luaState["__resourceEnvTable"];

	std::vector<ResourceId> resources;

	resourcePackEnv.set_function("shader", [this, &resources] (std::string_view name, std::string_view vertexShader, std::string_view fragmentShader) {
		auto shader = std::make_unique<Shader>(name);
		shader->rendererId = ShaderLoader::loadProgram(std::string(vertexShader), std::string(fragmentShader));
		resources.push_back(shader->id);
		_registerResource(std::move(shader));
	});

	resourcePackEnv.set_function("script", [this, &resourcePackEnv] (std::string_view definitionFile) {
		auto path = _pathHelper.makeAbsolutePath(definitionFile);
		_luaState.script_file(path, resourcePackEnv);
	});

	ScriptingHelper::executeFunction(_luaState, luaDefinitionLoader);

	resourcePackEnv["shader"] = sol::nil;
	resourcePackEnv["script"] = sol::nil;
	return resources;
}

void ShaderManager::unload(ResourceId resourceId) {
	auto& resource = getResource(resourceId);
	ShaderLoader::unloadProgram(resource.rendererId);
	_removeResource(resource);
}

void ShaderManager::unload(std::vector<ResourceId> resourceIds) {
	for (auto& resourceId : resourceIds) {
		unload(resourceId);
	}
}

}
