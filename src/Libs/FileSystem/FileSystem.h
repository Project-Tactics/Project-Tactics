#pragma once

#include "FileHandle/FileHandle.h"

#include <nlohmann/json.hpp>
#include <inicpp.h>
#include <filesystem>
#include <memory>
#include <string_view>
#include <vector>

namespace tactics {
class FileLoader;

class FileSystem {
public:
	FileSystem(std::unique_ptr<FileLoader> fileLoader, std::string_view dataPath);

	std::unique_ptr<FileHandle<ini::IniFile>> createIniFileHandle(std::string_view path);
	std::unique_ptr<FileHandle<std::string>> createStringFileHandle(std::string_view path);
	std::unique_ptr<FileHandle<nlohmann::ordered_json>> createJsonFileHandle(std::string_view path);

	bool dataPathExists(std::string_view path) const;
	std::string makeAbsolutePath(std::string_view path) const;

private:
	void _updateDataRelativePath(std::string_view dataPath);

	std::filesystem::path _dataRelativePath;
	std::filesystem::path _dataAbsolutePath;
	std::unique_ptr<FileLoader> _fileLoader;
};

}
