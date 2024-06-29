#include "PrefabLoader.h"

#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Ecs/EntityComponentSystem.h>
#include <Libs/Ecs/EntityUtilities.h>
#include <Libs/FileSystem/FileSystem.h>
#include <Libs/Utility/Exception.h>

#include <cereal/archives/json.hpp>

// TODO(Gerark) These are totally temporary functions, should be moved or removed entirely
void epilogue(cereal::JSONInputArchive&, const tactics::component::Transform&) {}
void prologue(cereal::JSONInputArchive&, const tactics::component::Transform&) {}

namespace tactics::resource {

PrefabLoader::PrefabLoader(FileSystem& fileSystem, const ResourceProvider& resourceProvider, EntityComponentSystem& ecs)
	: ResourceLoader(fileSystem, resourceProvider)
	, _ecs(ecs) {
}

std::shared_ptr<Prefab> PrefabLoader::load(HashId name, const FileDescriptor& descriptor) {
	auto prefab = std::make_shared<Prefab>(name);

	auto& fileSystem = _getFileSystem();
	auto jsonHandle = fileSystem.createJsonFileHandle(descriptor.path);
	jsonHandle->load();
	prefab->entity = _ecs.createPrefab(name, jsonHandle->getContent(), _getResourceProvider());
	return prefab;
}

}
