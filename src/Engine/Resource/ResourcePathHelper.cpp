#include "ResourcePathHelper.h"

namespace tactics {

ResourcePathHelper::ResourcePathHelper(std::string_view dataPath) {
	setRelativeDataPath(dataPath);
}

void ResourcePathHelper::setRelativeDataPath(std::string_view dataPath) {
	_absoluteRootDataPath = std::filesystem::current_path();
	_absoluteRootDataPath /= dataPath;
}

std::string ResourcePathHelper::makeAbsolutePath(std::string_view suffix) const {
	return (_absoluteRootDataPath / suffix).string();
}

}
