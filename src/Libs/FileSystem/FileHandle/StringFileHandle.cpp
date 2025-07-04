#include "StringFileHandle.h"

#include "Libs/Utility/Exception.h"

#include <fstream>

namespace tactics {

StringFileHandle::StringFileHandle(const std::filesystem::path& path) : _path(path) {}

bool StringFileHandle::exists() const {
	return std::filesystem::exists(_path);
}

void StringFileHandle::save() {
	std::ofstream file(_path);
	file << getContent();
}

void StringFileHandle::load() {
	std::ifstream file(_path);
	if (!file.is_open()) {
		TACTICS_EXCEPTION("Could not open file: {}", _path.string());
	}

	std::ostringstream oss;
	oss << file.rdbuf();
	_setContent(oss.str());
}

} // namespace tactics
