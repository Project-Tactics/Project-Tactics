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
void registerManager(ResourceSystem& resourceSystem) {
	resourceSystem.registerManager<ResourceManager<TResource, TResourceLoader>>();
}

std::unique_ptr<ResourceSystem> ResourceSystemInitializer::initialize() {
	auto resourceSystem = std::make_unique<resource::ResourceSystem>("data");
	registerManager<IniFile, IniFileLoader>(*resourceSystem);
	registerManager<Texture, TextureLoader>(*resourceSystem);
	registerManager<Shader, ShaderLoader>(*resourceSystem);
	registerManager<Mesh, MeshLoader>(*resourceSystem);
	registerManager<Material, MaterialLoader>(*resourceSystem);

	// Let's initialize some engine data
	resourceSystem->loadResourcePackDefinition("engine_data.json");
	resourceSystem->loadResourcePack("initialization");

	return resourceSystem;
}

}
