#include "PathHelper.h"

namespace tactics {

PathHelper::PathHelper(const std::filesystem::path& dataPath) { _updateDataRelativePath(dataPath); }

void PathHelper::_updateDataRelativePath(const std::filesystem::path& dataPath) {
	_dataRelativePath = dataPath;
	_dataAbsolutePath = std::filesystem::current_path();
	_dataAbsolutePath /= dataPath;
}

std::string PathHelper::makeAbsolutePath(const std::filesystem::path& path) const {
	return (_dataAbsolutePath / path).string();
}

} // namespace tactics
