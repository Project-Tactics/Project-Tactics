#include "FileLoader.h"

#include "FileHandle/IniFileHandle.h"
#include "FileHandle/JsonFileHandle.h"
#include "FileHandle/StringFileHandle.h"

#include <Libs/Utility/Exception.h>

#include <fstream>

namespace tactics {

DefaultFileLoader::DefaultFileLoader(PathHelper& pathHelper) : _pathHelper(pathHelper) {}

std::unique_ptr<FileHandle<std::string>> DefaultFileLoader::createStringFile(const std::filesystem::path& path) const {
	return std::make_unique<StringFileHandle>(path);
}

std::unique_ptr<FileHandle<nlohmann::ordered_json>>
DefaultFileLoader::createJsonFile(const std::filesystem::path& path) const {
	return std::make_unique<JsonFileHandle>(path, _pathHelper);
}

std::unique_ptr<FileHandle<ini::IniFile>> DefaultFileLoader::createIni(const std::filesystem::path& path) const {
	return std::make_unique<IniFileHandle>(path);
}

} // namespace tactics
