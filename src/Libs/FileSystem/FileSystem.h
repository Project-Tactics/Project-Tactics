#pragma once

#include "FileHandle/FileHandle.h"
#include "PathHelper.h"

#include <filesystem>
#include <inicpp.h>
#include <memory>
#include <nlohmann/json.hpp>
#include <string_view>
#include <vector>

namespace tactics {
class FileLoader;

class FileSystem {
public:
	FileSystem(std::unique_ptr<FileLoader> fileLoader, std::unique_ptr<PathHelper> dataPath);

	std::unique_ptr<FileHandle<ini::IniFile>> createIniFileHandle(const std::filesystem::path& path);
	std::unique_ptr<FileHandle<std::string>> createStringFileHandle(const std::filesystem::path& path);
	std::unique_ptr<FileHandle<nlohmann::ordered_json>> createJsonFileHandle(const std::filesystem::path& path);

	const PathHelper& getPathHelper() const;

private:
	std::unique_ptr<PathHelper> _pathHelper;
	std::unique_ptr<FileLoader> _fileLoader;
};

} // namespace tactics
