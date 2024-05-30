#pragma once

#include <memory>

namespace tactics {
class FileSystem;
}

namespace tactics::resource {

class ResourceSystem;

class ResourceSystemInitializer {
public:
	static std::unique_ptr<ResourceSystem> initialize(FileSystem& fileSystem);
};

}
