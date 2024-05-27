#pragma once

#include <memory>

namespace tactics::resource {

class ResourceSystem;

class ResourceSystemInitializer {
public:
	static std::unique_ptr<ResourceSystem> initialize();
};

}
