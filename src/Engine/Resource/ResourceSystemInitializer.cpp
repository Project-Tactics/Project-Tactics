#include "ResourceSystemInitializer.h"

#include "Texture/TextureManager.h"
#include "Shader/ShaderManager.h"
#include "Mesh/MeshManager.h"

#include <Libs/Resource/ResourceSystem.h>
#include <Libs/Resource/IniFile/IniFileManager.h>

namespace tactics {

void ResourceSystemInitializer::initialize(ResourceSystem& resourceSystem) {
	resourceSystem.registerManager<IniFileManager>();
	resourceSystem.registerManager<TextureManager>();
	resourceSystem.registerManager<ShaderManager>();
	resourceSystem.registerManager<MeshManager>();
}

}
