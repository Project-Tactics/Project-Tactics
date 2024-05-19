#include "ResourceSystemInitializer.h"

#include "Texture/TextureManager.h"
#include "Shader/ShaderManager.h"

#include <Libs/Resource/ResourceSystem.h>

namespace tactics {

void ResourceSystemInitializer::initialize(ResourceSystem& resourceSystem) {
	resourceSystem.registerManager<TextureManager>();
	resourceSystem.registerManager<ShaderManager>();
}

}
