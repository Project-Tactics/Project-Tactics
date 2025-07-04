#include "JsonFileHandle.h"

#include "../PathHelper.h"
#include "StringFileHandle.h"

#include <Libs/Utility/Exception.h>

#include <fstream>

namespace tactics {

JsonFileHandle::JsonFileHandle(const std::filesystem::path& path, PathHelper& pathHelper)
	: _path(path)
	, _pathHelper(pathHelper) {
	if (!std::filesystem::exists(_path)) {
		TACTICS_EXCEPTION("JsonFileHandle: File [{}] does not exist", _path.string());
	}
	if (std::filesystem::is_empty(_path)) {
		TACTICS_EXCEPTION("JsonFileHandle: File [{}] is empty", _path.string());
	}
}

bool JsonFileHandle::exists() const {
	return std::filesystem::exists(_path);
}

void JsonFileHandle::save() {
	std::ofstream file(_path);
	file << getContent().dump(4);
}

void JsonFileHandle::load() {
	_loadFromJson();
}

void JsonFileHandle::_loadFromJson() {
	std::ifstream stream(_path);
	_load(stream);
}

void JsonFileHandle::_load(std::istream& stream) {
	try {
		_setContent(ordered_json::parse(stream));
	} catch (json::parse_error& error) {
		TACTICS_EXCEPTION("Failed to parse json file [{}]. Error: {}", _path.string(), error.what());
	}
}

} // namespace tactics
