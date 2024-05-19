#include "IniFileManager.h"

#include <Libs/Resource/ResourcePathHelper.h>
#include <Libs/Scripting/ScriptingHelper.h>

#include <filesystem>

namespace tactics {

std::vector<ResourceId> IniFileManager::load(sol::reference& luaDefinitionLoader) {
	sol::environment resourcePackEnv = _luaState["__resourceEnvTable"];

	std::vector<ResourceId> resources;

	resourcePackEnv.set_function("iniDef", [this, &resources] (std::string_view name, std::string_view definitionFile) {
		auto path = _pathHelper.makeAbsolutePath(definitionFile);
		auto iniFile = std::make_unique<IniFile>(name);
		iniFile->filename = path;
		if (std::filesystem::exists(path)) {
			iniFile->file.load(path);
		} else {
			iniFile->file.save(path);
		}
		resources.push_back(iniFile->id);
		_registerResource(std::move(iniFile));
	});

	ScriptingHelper::executeFunction(_luaState, luaDefinitionLoader);

	resourcePackEnv["iniDef"] = sol::nil;
	return resources;
}

void IniFileManager::unload(ResourceId resourceId) {
	auto resource = getResource(resourceId);
	resource->save();
	_removeResource(resource);
}

void IniFileManager::unload(std::vector<ResourceId> resourceIds) {
	for (auto resourceId : resourceIds) {
		unload(resourceId);
	}
}

}