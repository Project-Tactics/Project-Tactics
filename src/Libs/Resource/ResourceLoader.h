#pragma once

#include "ResourcePathHelper.h"

namespace tactics::resource {

class ResourceLoader {
public:
	ResourceLoader(const ResourcePathHelper& pathHelper): _pathHelper(pathHelper) {}

protected:
	std::string _makeAbsolutePath(const std::string& path) {
		return _pathHelper.makeAbsolutePath(path);
	}

private:
	const ResourcePathHelper& _pathHelper;
};

}
