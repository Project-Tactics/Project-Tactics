#include "IniFile.h"

namespace tactics::resource {

ini::IniSection& IniFile::createSection(std::string_view section) {
	auto& file = fileHandle->getObject();
	if (!file.contains(section.data())) {
		file[section.data()] = {};
	}
	return file[section.data()];
}

void IniFile::merge(ini::IniFile& other) {
	fileHandle->getObject().merge(other);
	for (auto& [otherSectionName, otherSection] : other) {
		auto& thisSection = fileHandle->getObject()[otherSectionName];
		thisSection.merge(otherSection);
	}
}

}
