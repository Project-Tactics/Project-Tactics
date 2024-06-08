#pragma once

#include "../ResourceLoader.h"

#include "Prefab.h"

#include <entt/entt.hpp>

namespace tactics {
class EntityComponentSystem;

namespace resource {

class PrefabLoader: public ResourceLoader {
public:
	PrefabLoader(FileSystem& fileSystem, const ResourceProvider& resourceProvider, EntityComponentSystem& ecs);
	std::shared_ptr<Prefab> load(const std::string& name, const FileDescriptor& descriptor);
private:
	void _fillComponentThroughReflection(entt::meta_any& anyValue, const nlohmann::ordered_json& jsonValue);

	EntityComponentSystem& _ecs;
};

}
}
