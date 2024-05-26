#include "ResourceSystemInitializer.h"

#include "Texture/TextureLoader.h"
#include "Shader/ShaderLoader.h"
#include "Mesh/MeshLoader.h"
#include "Material/MaterialLoader.h"

#include <Libs/Resource/ResourceSystem.h>
#include <Libs/Resource/IniFile/IniFileLoader.h>

namespace tactics::resource {

// Little helper function to keep the registration way more clean
template<typename TResource, typename TResourceLoader>
void registerManager(ResourceSystem& resourceSystem) {
	resourceSystem.registerManager<ResourceManager<TResource, TResourceLoader>>();
}

void ResourceSystemInitializer::initialize(ResourceSystem& resourceSystem) {
	registerManager<IniFile, IniFileLoader>(resourceSystem);
	registerManager<Texture, TextureLoader>(resourceSystem);
	registerManager<Shader, ShaderLoader>(resourceSystem);
	registerManager<Mesh, MeshLoader>(resourceSystem);
	registerManager<Material, MaterialLoader>(resourceSystem);
}

}
