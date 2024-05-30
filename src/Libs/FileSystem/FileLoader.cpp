#include "FileLoader.h"

#include "FileHandle/IniFileHandle.h"
#include "FileHandle/JsonFileHandle.h"
#include "FileHandle/StringFileHandle.h"

#include <Libs/Utility/Exception.h>

#include <fstream>

namespace tactics {

std::unique_ptr<FileHandle<std::string>> DefaultFileLoader::createStringFile(std::string_view path) const {
	return std::make_unique<StringFileHandle>(path);
}

std::unique_ptr<FileHandle<nlohmann::ordered_json>> DefaultFileLoader::createJsonFile(std::string_view path) const {
	return std::make_unique<JsonFileHandle>(path);
}

std::unique_ptr<FileHandle<ini::IniFile>> DefaultFileLoader::createIni(std::string_view path) const {
	return std::make_unique<IniFileHandle>(path);
}

}
