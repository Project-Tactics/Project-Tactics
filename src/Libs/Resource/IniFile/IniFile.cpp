#include "IniFile.h"

namespace tactics {

void IniFile::save() {
	file.save(filename);
}

void IniFile::reload() {
	file.load(filename);
}

ini::IniSection& IniFile::createSection(std::string_view section) {
	if (!file.contains(section.data())) {
		file[section.data()] = {};
	}
	return file[section.data()];
}

}
