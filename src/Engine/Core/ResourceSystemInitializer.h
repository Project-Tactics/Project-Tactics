#pragma once

#include <memory>

namespace tactics {
class FileSystem;
}

namespace tactics {
namespace resource {
class ResourceSystem;
}

class ResourceSystemInitializer {
public:
	static std::unique_ptr<resource::ResourceSystem> initialize(FileSystem& fileSystem);
};

}
