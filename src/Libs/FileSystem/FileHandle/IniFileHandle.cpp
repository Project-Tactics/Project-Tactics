#include "IniFileHandle.h"

#include <Libs/Utility/Exception.h>

namespace tactics {

IniFileHandle::IniFileHandle(const std::filesystem::path& path): _path(path) {
}

IniFileHandle::~IniFileHandle() {
	save();
}

void IniFileHandle::load() {
	std::ifstream file(_path);
	if (!file.is_open()) {
		throw Exception("Could not open file: {}", _path.string());
	}
	getObject().decode(file);
}

void IniFileHandle::save() {
	std::ofstream file(_path);
	if (!file.is_open()) {
		throw Exception("Could not open file: {}", _path.string());
	}
	getObject().encode(file);
}

bool IniFileHandle::exists() const {
	return std::filesystem::exists(_path);
}

}
