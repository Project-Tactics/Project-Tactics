#pragma once

#include "FileHandle/FileHandle.h"

#include <Libs/Utility/Json/Json.h>

#include <inicpp.h>
#include <memory>
#include <string>
#include <string_view>

namespace tactics {
class PathHelper;

class FileLoader {
public:
	virtual ~FileLoader() = default;
	virtual std::unique_ptr<FileHandle<std::string>> createStringFile(const std::filesystem::path& path) const = 0;
	virtual std::unique_ptr<FileHandle<ordered_json>> createJsonFile(const std::filesystem::path& path) const = 0;
	virtual std::unique_ptr<FileHandle<ini::IniFile>> createIni(const std::filesystem::path& path) const = 0;
};

class DefaultFileLoader : public FileLoader {
public:
	DefaultFileLoader(PathHelper& pathHelper);
	std::unique_ptr<FileHandle<ini::IniFile>> createIni(const std::filesystem::path& path) const override;
	std::unique_ptr<FileHandle<std::string>> createStringFile(const std::filesystem::path& path) const override;
	std::unique_ptr<FileHandle<ordered_json>> createJsonFile(const std::filesystem::path& path) const override;

private:
	PathHelper& _pathHelper;
};

} // namespace tactics
