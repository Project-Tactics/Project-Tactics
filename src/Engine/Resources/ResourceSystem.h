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

	BaseResource* getResource(ResourceId id);

	template<typename TResource>
	Resource<TResource>* as(ResourceId id) {
		return static_cast<Resource<TResource>*>(getResource(id));
	}

	std::shared_ptr<ShaderResource> defineShader(std::string_view name, std::string_view vertexShader, std::string_view fragmentShader);

private:
	void _loadShaders();
	std::string _makeAbsolutePath(std::string_view path);

	std::unordered_map<ResourceId, std::shared_ptr<BaseResource>> _resources;
	std::unique_ptr<sol::state> _luaState;
	std::filesystem::path _absoluteDataPath;
};

}
