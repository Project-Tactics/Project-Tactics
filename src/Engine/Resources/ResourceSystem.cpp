#include "ResourceSystem.h"
#include "ShaderLoader.h"

#include <Libs/Utilities/Exception.h>
#include <Libs/Utilities/UUID.h>

#include <sol/sol.hpp>
#include <fmt/format.h>
#include <algorithm>
#include <ranges>

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

std::shared_ptr<ShaderResource> ResourceSystem::getShader(std::string_view name) {
	auto resource = getResource(name);
	if (resource) {
		return std::static_pointer_cast<ShaderResource>(resource);
	}

	return nullptr;
}

std::shared_ptr<BaseResource> ResourceSystem::getResource(ResourceId id) {
	if (!_resources.contains(id)) {
		throw Exception("Resource with id {} does not exists. Can't find resource.", id);
	}

	return _resources[id];
}

std::shared_ptr<BaseResource> ResourceSystem::getResource(std::string_view name) {
	auto itr = std::ranges::find_if(_resources, [name] (const auto& pair) {
		return pair.second->name == name;
	});

	if (itr == _resources.end()) {
		throw Exception("Resource with name {} does not exists. Can't find resource.", name);
	}

	return itr->second;
}

void ResourceSystem::_loadShaders() {
	// Let's create a lua environment to expose only the functions required when loading shaders
	sol::environment shaderEnv(*_luaState, sol::create);

	// Expose the shader function to lua to give the possibility to define new shaders
	shaderEnv.set_function("shader", [this] (std::string name, std::string vertexShader, std::string fragmentShader) {
		createShader(name, vertexShader, fragmentShader);
	});

	auto path = _makeAbsolutePath("shaders/shader_definition.lua");
	_luaState->script_file(path, shaderEnv);
}

ResourceId ResourceSystem::createShader(const std::string& name, const std::string& vertexShader, const std::string& fragmentShader) {
	auto shader = std::make_shared<ShaderResource>();
	shader->id = generateUUID();
	shader->type = ResourceType::Shader;
	shader->name = name;
	shader->shaderId = ShaderLoader::loadProgram(vertexShader, fragmentShader);
	_resources.insert({shader->id, shader});
	return shader->id;
}

std::string ResourceSystem::_makeAbsolutePath(std::string_view dataPath) {
	return (_absoluteDataPath / dataPath).string();
}

}
