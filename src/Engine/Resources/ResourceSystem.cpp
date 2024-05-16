#include "ResourceSystem.h"

#include <Libs/Utilities/Exception.h>
#include <Libs/Utilities/UUID.h>

#include <sol/sol.hpp>
#include <fmt/format.h>

namespace tactics {

ResourceSystem::ResourceSystem(std::string_view relativeDataPath) {
	_absoluteDataPath = std::filesystem::current_path();
	_absoluteDataPath /= relativeDataPath;

	_luaState = std::make_unique<sol::state>();
	_luaState->open_libraries(sol::lib::base);
}

ResourceSystem::~ResourceSystem() {
}

void ResourceSystem::loadResources() {
	_loadShaders();
}

BaseResource* ResourceSystem::getResource(ResourceId id) {
	if (!_resources.contains(id)) {
		throw Exception("Resource with it id {} does not exists. Can't find resource.", id);
	}

	return _resources[id].get();
}

void ResourceSystem::_loadShaders() {
	// Let's create a lua environment to expose only the functions required when loading shaders
	sol::environment shaderEnv(*_luaState, sol::create);

	// Expose the shader function to lua to give the possibility to define new shaders
	shaderEnv.set_function("shader", [this] (std::string_view name, std::string_view vertexShader, std::string_view fragmentShader) {
		auto shader = defineShader(name, vertexShader, fragmentShader);
		_resources.insert({shader->id, shader});
	});

	auto path = _makeAbsolutePath("shaders/shader_definition.lua");
	_luaState->script_file(path, shaderEnv);
}

std::shared_ptr<ShaderResource> ResourceSystem::defineShader(std::string_view name, std::string_view vertexShader, std::string_view fragmentShader) {
	auto shader = std::make_shared<ShaderResource>();
	shader->id = generateUUID();
	shader->type = ResourceType::Shader;
	shader->name = name;
	shader->fragmentShader = fragmentShader;
	shader->vertexShader = vertexShader;
	return shader;
}


std::string ResourceSystem::_makeAbsolutePath(std::string_view dataPath) {
	return (_absoluteDataPath / dataPath).string();
}

}
