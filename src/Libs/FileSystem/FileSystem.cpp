#include "FileSystem.h"

#include "FileLoader.h"

namespace tactics {

FileSystem::FileSystem(std::unique_ptr<FileLoader> fileLoader, std::string_view dataPath): _fileLoader(std::move(fileLoader)) {
	_updateDataRelativePath(dataPath);
}

void FileSystem::_updateDataRelativePath(std::string_view dataPath) {
	_dataRelativePath = dataPath;
	_dataAbsolutePath = std::filesystem::current_path();
	_dataAbsolutePath /= dataPath;
}

std::string FileSystem::makeAbsolutePath(std::string_view path) const {
	return (_dataAbsolutePath / path).string();
}

bool FileSystem::dataPathExists(std::string_view path) const {
	return std::filesystem::exists(makeAbsolutePath(path));
}

std::unique_ptr<FileHandle<ini::IniFile>> FileSystem::createIniFileHandle(std::string_view path) {
	return _fileLoader->createIni(makeAbsolutePath(path));
}

std::unique_ptr<FileHandle<std::string>> FileSystem::createStringFileHandle(std::string_view path) {
	return _fileLoader->createStringFile(makeAbsolutePath(path));
}

std::unique_ptr<FileHandle<nlohmann::ordered_json>> FileSystem::createJsonFileHandle(std::string_view path) {
	return _fileLoader->createJsonFile(makeAbsolutePath(path));
}

}
