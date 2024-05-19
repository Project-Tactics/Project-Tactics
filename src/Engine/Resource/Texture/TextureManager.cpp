#include "TextureManager.h"

#include "TextureLoader.h"

#include <Libs/Resource/ResourcePathHelper.h>
#include <Libs/Scripting/ScriptingHelper.h>

namespace tactics {

std::vector<ResourceId> TextureManager::load(sol::reference& luaDefinitionLoader) {
	sol::environment resourcePackEnv = _luaState["__resourceEnvTable"];

	std::vector<ResourceId> resources;

	resourcePackEnv.set_function("texture", [this, &resources] (std::string_view name, std::string_view filename) {
		auto texture = std::make_unique<Texture>(name);
		auto [textureId, info] = TextureLoader::loadTexture(_pathHelper.makeAbsolutePath(filename).c_str());
		texture->rendererId = textureId;
		texture->info = info;
		resources.push_back(texture->id);
		_registerResource(std::move(texture));
	});

	resourcePackEnv.set_function("textureDef", [this, &resourcePackEnv] (std::string_view definitionFile) {
		auto path = _pathHelper.makeAbsolutePath(definitionFile);
		_luaState.script_file(path, resourcePackEnv);
	});

	ScriptingHelper::executeFunction(_luaState, luaDefinitionLoader);

	resourcePackEnv["texture"] = sol::nil;
	resourcePackEnv["textureDef"] = sol::nil;
	return resources;
}

void TextureManager::unload(ResourceId resourceId) {
	auto resource = getResource(resourceId);
	TextureLoader::unloadTexture(resource->rendererId);
	_removeResource(resource);
}

void TextureManager::unload(std::vector<ResourceId> resourceIds) {
	for (auto& resourceId : resourceIds) {
		unload(resourceId);
	}

}

}