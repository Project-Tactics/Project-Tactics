#include "ResourceSystemInitializer.h"

#include "Texture/TextureLoader.h"
#include "Shader/ShaderLoader.h"
#include "Mesh/MeshLoader.h"
#include "Material/MaterialLoader.h"

#include <Libs/Resource/ResourceSystem.h>
#include <Libs/Resource/IniFile/IniFileLoader.h>

namespace tactics::resource {

// Little helper function to keep the registration of a Resource Manager way cleaner
template<typename TResource, typename TResourceLoader>
void registerManager(FileSystem& fileSystem, ResourceSystem& resourceSystem) {
	auto loader = std::make_unique<TResourceLoader>(fileSystem, resourceSystem);
	resourceSystem.registerManager(std::make_unique<ResourceManager<TResource, TResourceLoader>>(std::move(loader)));
}

std::unique_ptr<ResourceSystem> ResourceSystemInitializer::initialize(FileSystem& fileSystem) {
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
