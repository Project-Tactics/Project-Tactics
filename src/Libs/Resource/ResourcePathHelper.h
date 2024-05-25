#pragma once

#include <string>
#include <filesystem>

namespace tactics::resource {

/**
 * Utility class for managing and retrieving important filesystem paths and resources.
 */
class ResourcePathHelper {
public:
	ResourcePathHelper(std::string_view dataPath);

	void setRelativeDataPath(std::string_view dataPath);
	std::string makeAbsolutePath(std::string_view suffix) const;

private:
	std::filesystem::path _absoluteRootDataPath;
};

}
