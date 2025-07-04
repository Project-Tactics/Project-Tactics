#include "ResourceSystemInitializer.h"

#include <Libs/Resource/DataSet/DataSetLoader.h>
#include <Libs/Resource/IniFile/IniFileLoader.h>
#include <Libs/Resource/Input/InputActionLoader.h>
#include <Libs/Resource/Input/InputMapLoader.h>
#include <Libs/Resource/Material/MaterialLoader.h>
#include <Libs/Resource/Mesh/MeshLoader.h>
#include <Libs/Resource/ParticleEffect/ParticleEffectLoader.h>
#include <Libs/Resource/Prefab/PrefabLoader.h>
#include <Libs/Resource/ResourceSerialization.h>
#include <Libs/Resource/ResourceSystem.h>
#include <Libs/Resource/Shader/ShaderLoader.h>
#include <Libs/Resource/SpriteSheet/SpriteSheetLoader.h>
#include <Libs/Resource/Texture/TextureLoader.h>

namespace tactics {

template<typename TResource, typename TResourceLoader, typename... TArgs>
void registerManager(FileSystem& fileSystem, resource::ResourceSystem& resourceSystem, TArgs&&... args) {
	auto loader = std::make_unique<TResourceLoader>(fileSystem, resourceSystem, std::forward<TArgs>(args)...);
	resourceSystem.registerManager(
		std::make_unique<resource::ResourceManager<TResource, TResourceLoader>>(std::move(loader)));
}

std::unique_ptr<resource::ResourceSystem> ResourceSystemInitializer::initialize(FileSystem& fileSystem,
																				EntityComponentSystem& ecs) {
	using namespace resource;
	auto resourceSystem = std::make_unique<resource::ResourceSystem>(fileSystem);
	setResourceProviderForSerialization(resourceSystem.get());
	registerManager<IniFile, IniFileLoader>(fileSystem, *resourceSystem);
	registerManager<InputAction, InputActionLoader>(fileSystem, *resourceSystem);
	registerManager<InputMap, InputMapLoader>(fileSystem, *resourceSystem);
	registerManager<Texture, TextureLoader>(fileSystem, *resourceSystem);
	registerManager<Shader, ShaderLoader>(fileSystem, *resourceSystem);
	registerManager<Mesh, MeshLoader>(fileSystem, *resourceSystem);
	registerManager<Material, MaterialLoader>(fileSystem, *resourceSystem);
	registerManager<SpriteSheet, SpriteSheetLoader>(fileSystem, *resourceSystem);
	registerManager<Prefab, PrefabLoader>(fileSystem, *resourceSystem, ecs);
	registerManager<ParticleEffect, ParticleEffectLoader>(fileSystem, *resourceSystem);
	registerManager<BaseDataSet, DataSetLoader>(fileSystem, *resourceSystem);

	resourceSystem->loadPackDefinition("engine_data.json");
	resourceSystem->loadPack("initialization"_id);

	return resourceSystem;
}

} // namespace tactics
