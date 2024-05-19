#include "ResourceSystem.h"

#include "Texture/TextureManager.h"
#include "Shader/ShaderManager.h"
#include "ResourcePack/ResourcePackManager.h"

#include <Libs/Utilities/Exception.h>
#include <Libs/Scripting/ScriptingHelper.h>

#include <sol/sol.hpp>
#include <fmt/format.h>
#include <algorithm>
#include <ranges>

namespace tactics {

ResourceSystem::ResourceSystem(std::string_view relativeDataPath): _resourcePathHelper(relativeDataPath) {
	// TODO(Gerark) We're currently creating a lua state here but the idea is to inject it from outside.
	_luaState = std::make_unique<sol::state>();
	_luaState->open_libraries(sol::lib::base);
	// let's create the environment table for the resources
	sol::environment resourceEnv(*_luaState, sol::create);
	(*_luaState)["__resourceEnvTable"] = resourceEnv;

	auto managerProvider = [this] (ResourceType resourceType) {
		auto itr = _resourceManagers.find(resourceType);
		if (itr == _resourceManagers.end()) {
			throw Exception("Can't find manager for resource type: {}",
				ResourceTypeSerialization::toString(resourceType));
		}
		return itr->second.get();
	};

	sol::state_view luaStateView(*_luaState);
	_resourcePackManager = std::make_unique<ResourcePackManager>(luaStateView, _resourcePathHelper, managerProvider);
	auto textureManager = std::make_unique<TextureManager>(luaStateView, _resourcePathHelper);
	auto shaderManager = std::make_unique<ShaderManager>(luaStateView, _resourcePathHelper);

	_registerManager(std::move(textureManager));
	_registerManager(std::move(shaderManager));
}

ResourceSystem::~ResourceSystem() {
	_cleanupResources();
}

void ResourceSystem::loadResourcePackDefinition(std::string_view definitionPath) {
	_resourcePackManager->loadPackDefinition(definitionPath);
}

void ResourceSystem::loadResourcePack(std::string_view resourcePackName) {
	_resourcePackManager->loadPack(resourcePackName);
}

void ResourceSystem::_cleanupResources() {
	_resourcePackManager->unloadAllPacks();
}

void ResourceSystem::_registerManager(std::unique_ptr<ResourceManager> resourceManager) {
	auto type = resourceManager->getType();
	if (_resourceManagers.contains(type)) {
		throw Exception("Can't register a new Resource Type Manager for resource of type {}. A manager is already registered.",
			ResourceTypeSerialization::toString(type));
	}

	_resourceManagers.insert({type, std::move(resourceManager)});
}

void ResourceSystem::_unregisterManager(std::unique_ptr<ResourceManager> resourceManager) {
	auto itr = _resourceManagers.find(resourceManager->getType());
	if (itr == _resourceManagers.end()) {
		throw Exception("Can't register a new Resource Type Manager for resource of type {}. A manager is already registered.",
			ResourceTypeSerialization::toString(resourceManager->getType()));
	}

	_resourceManagers.erase(itr);
}

}
