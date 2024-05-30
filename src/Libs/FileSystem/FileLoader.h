#pragma once

#include "FileHandle/FileHandle.h"

#include <inicpp.h>
#include <memory>
#include <string>
#include <string_view>
#include <nlohmann/json.hpp>

namespace tactics {

class FileLoader {
public:
	virtual ~FileLoader() = default;
	virtual std::unique_ptr<FileHandle<std::string>> createStringFile(std::string_view path) const = 0;
	virtual std::unique_ptr<FileHandle<nlohmann::ordered_json>> createJsonFile(std::string_view path) const = 0;
	virtual std::unique_ptr<FileHandle<ini::IniFile>> createIni(std::string_view path) const = 0;
};

class DefaultFileLoader: public FileLoader {
public:
	std::unique_ptr<FileHandle<ini::IniFile>> createIni(std::string_view path) const override;
	std::unique_ptr<FileHandle<std::string>> createStringFile(std::string_view path) const override;
	std::unique_ptr<FileHandle<nlohmann::ordered_json>> createJsonFile(std::string_view path) const override;
};

}
