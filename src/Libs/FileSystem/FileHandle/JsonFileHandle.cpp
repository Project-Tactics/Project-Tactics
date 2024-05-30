#include "JsonFileHandle.h"

#include <fstream>

namespace tactics {

JsonFileHandle::JsonFileHandle(const std::filesystem::path& path): _path(path) {}

bool JsonFileHandle::exists() const {
	return std::filesystem::exists(_path);
}

void JsonFileHandle::save() {
	std::ofstream file(_path);
	file << getObject().dump(4);
}

void JsonFileHandle::load() {
	std::ifstream file(_path);
	_setObject(nlohmann::ordered_json::parse(file));
}

}
