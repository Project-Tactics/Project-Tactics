#pragma once

#include <memory>

namespace tactics {
class EntityComponentSystem;
class FileSystem;

namespace resource {
class ResourceSystem;
}

class ResourceSystemInitializer {
public:
	static std::unique_ptr<resource::ResourceSystem> initialize(FileSystem& fileSystem, EntityComponentSystem& ecs);
};

}
