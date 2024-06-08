#pragma once

#include <filesystem>

namespace tactics {

class PathHelper {
public:
	PathHelper(const std::filesystem::path& dataPath);

	std::string makeAbsolutePath(const std::filesystem::path& path) const;

private:
	void _updateDataRelativePath(const std::filesystem::path& dataPath);

	std::filesystem::path _dataRelativePath;
	std::filesystem::path _dataAbsolutePath;
};

}
