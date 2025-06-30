#include "PrefabLoader.h"

#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Ecs/EntityComponentSystem.h>
#include <Libs/Ecs/EntityUtilities.h>
#include <Libs/FileSystem/FileSystem.h>
#include <Libs/Utility/Exception.h>

#include <cereal/archives/json.hpp>

namespace tactics::resource {

PrefabLoader::PrefabLoader(FileSystem& fileSystem, const ResourceProvider& resourceProvider, EntityComponentSystem& ecs)
	: ResourceLoader(fileSystem, resourceProvider)
	, _ecs(ecs) {}

std::shared_ptr<Prefab> PrefabLoader::load(HashId name, const FileDescriptor& descriptor) {
	auto prefab = std::make_shared<Prefab>(name);

	auto& fileSystem = _getFileSystem();
	auto jsonHandle = fileSystem.createJsonFileHandle(descriptor.path);
	jsonHandle->load();
	prefab->entity = _ecs.createPrefab(name, jsonHandle->getContent(), _getResourceProvider());
	return prefab;
}

} // namespace tactics::resource
