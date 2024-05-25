#pragma once

namespace tactics::resource {

class ResourceSystem;

class ResourceSystemInitializer {
public:
	static void initialize(ResourceSystem& resourceSystem);
};

}
