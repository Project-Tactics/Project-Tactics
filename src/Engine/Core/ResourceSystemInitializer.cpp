#include "ResourceSystemInitializer.h"

#include <Libs/Resource/IniFile/IniFileLoader.h>
#include <Libs/Resource/Material/MaterialLoader.h>
#include <Libs/Resource/Mesh/MeshLoader.h>
#include <Libs/Resource/ResourceSystem.h>
#include <Libs/Resource/Shader/ShaderLoader.h>
#include <Libs/Resource/Texture/TextureLoader.h>

namespace tactics {

// Little helper function to keep the registration of a Resource Manager way cleaner
template<typename TResource, typename TResourceLoader>
void registerManager(FileSystem& fileSystem, resource::ResourceSystem& resourceSystem) {
	auto loader = std::make_unique<TResourceLoader>(fileSystem, resourceSystem);
	resourceSystem.registerManager(std::make_unique<resource::ResourceManager<TResource, TResourceLoader>>(std::move(loader)));
}

std::unique_ptr<resource::ResourceSystem> ResourceSystemInitializer::initialize(FileSystem& fileSystem) {
	using namespace resource;
	auto resourceSystem = std::make_unique<resource::ResourceSystem>(fileSystem);
	registerManager<IniFile, IniFileLoader>(fileSystem, *resourceSystem);
	registerManager<Texture, TextureLoader>(fileSystem, *resourceSystem);
	registerManager<Shader, ShaderLoader>(fileSystem, *resourceSystem);
	registerManager<Mesh, MeshLoader>(fileSystem, *resourceSystem);
	registerManager<Material, MaterialLoader>(fileSystem, *resourceSystem);

	// Let's initialize some engine data
	resourceSystem->loadPackDefinition("engine_data.json");
	resourceSystem->loadPack("initialization");

	return resourceSystem;
}

}
