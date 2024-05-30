#include "StringFileHandle.h"

#include "Libs/Utility/Exception.h"

#include <fstream>

namespace tactics {

StringFileHandle::StringFileHandle(const std::filesystem::path& path): _path(path) {}

bool StringFileHandle::exists() const {
	return std::filesystem::exists(_path);
}

void StringFileHandle::save() {
	std::ofstream file(_path);
	file << getObject();
}

void StringFileHandle::load() {
	std::ifstream file(_path);
	if (!file.is_open()) {
		throw Exception("Could not open file: {}", _path.string());
	}

	std::string content;
	file.seekg(0, std::ios::end);
	content.resize(file.tellg());
	file.seekg(0, std::ios::beg);
	file.read(content.data(), content.size());
	file.close();

	_setObject(content);
}

}
