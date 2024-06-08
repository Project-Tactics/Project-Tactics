#include "JsonFileHandle.h"

#include <Libs/Utility/Exception.h>

#include <fstream>

namespace tactics {

JsonFileHandle::JsonFileHandle(const std::filesystem::path& path): _path(path) {
	if (!std::filesystem::exists(_path)) {
		throw TACTICS_EXCEPTION("Json file [{}] does not exist", _path.string());
	}
	if (std::filesystem::is_empty(_path)) {
		throw TACTICS_EXCEPTION("Json file [{}] is empty", _path.string());
	}
}

bool JsonFileHandle::exists() const {
	return std::filesystem::exists(_path);
}

void JsonFileHandle::save() {
	std::ofstream file(_path);
	file << getObject().dump(4);
}

void JsonFileHandle::load() {
	std::ifstream file(_path);
	try {
		_setObject(nlohmann::ordered_json::parse(file));
	}
	catch (nlohmann::json::parse_error& error) {
		throw TACTICS_EXCEPTION("Failed to parse json file [{}]. Error: {}", _path.string(), error.what());
	}
}

}
