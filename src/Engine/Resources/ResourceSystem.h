#pragma once

#include "Resource.h"
#include "ShaderResource.h"

#include <filesystem>
#include <memory>
#include <string_view>
#include <unordered_map>

namespace sol {
class state;
}

namespace tactics {

/**
 * @brief A very raw Resource Management class that might be changed in the near future.
 *
 * This class handles the loading and management of various resources such as textures, shaders, etc...
 */

class ResourceSystem {
public:
	ResourceSystem(std::string_view relativeDataPath);
	~ResourceSystem();
	void loadResources();

	std::shared_ptr<BaseResource> getResource(ResourceId id);
	std::shared_ptr<BaseResource> getResource(std::string_view name);

	std::shared_ptr<ShaderResource> getShader(std::string_view name);

	ResourceId createShader(const std::string& name, const std::string& vertexShader, const std::string& fragmentShader);

private:
	void _loadShaders();
	std::string _makeAbsolutePath(std::string_view path);

	std::unordered_map<ResourceId, std::shared_ptr<BaseResource>> _resources;
	std::unique_ptr<sol::state> _luaState;
	std::filesystem::path _absoluteDataPath;
};

}
