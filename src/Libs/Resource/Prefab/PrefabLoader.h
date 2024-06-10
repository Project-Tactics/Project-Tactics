#pragma once

#include "../ResourceLoader.h"

#include "Prefab.h"

#include <Libs/Ecs/Entity.h>

namespace tactics {
class EntityComponentSystem;

namespace resource {

class PrefabLoader: public ResourceLoader {
public:
	PrefabLoader(FileSystem& fileSystem, const ResourceProvider& resourceProvider, EntityComponentSystem& ecs);
	[[nodiscard]] std::shared_ptr<Prefab> load(const std::string& name, const FileDescriptor& descriptor);
private:
	EntityComponentSystem& _ecs;
};

}
}
