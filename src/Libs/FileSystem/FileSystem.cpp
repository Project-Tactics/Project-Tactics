#include "FileSystem.h"

#include "FileLoader.h"

namespace tactics {

FileSystem::FileSystem(std::unique_ptr<FileLoader> fileLoader, std::unique_ptr<PathHelper> pathHelper)
	: _fileLoader(std::move(fileLoader))
	, _pathHelper(std::move(pathHelper)) {
}

const PathHelper& FileSystem::getPathHelper() const {
	return *_pathHelper;
}

std::unique_ptr<FileHandle<ini::IniFile>> FileSystem::createIniFileHandle(const std::filesystem::path& path) {
	return _fileLoader->createIni(_pathHelper->makeAbsolutePath(path));
}

std::unique_ptr<FileHandle<std::string>> FileSystem::createStringFileHandle(const std::filesystem::path& path) {
	return _fileLoader->createStringFile(_pathHelper->makeAbsolutePath(path));
}

std::unique_ptr<FileHandle<nlohmann::ordered_json>> FileSystem::createJsonFileHandle(const std::filesystem::path& path) {
	return _fileLoader->createJsonFile(_pathHelper->makeAbsolutePath(path));
}

}
