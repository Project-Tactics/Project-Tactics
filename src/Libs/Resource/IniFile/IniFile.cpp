#include "IniFile.h"

namespace tactics::resource {

ini::IniSection& IniFile::createSection(std::string_view section) {
	auto& file = fileHandle->getContent();
	if (!file.contains(section.data())) {
		file[section.data()] = {};
	}
	return file[section.data()];
}

void IniFile::merge(ini::IniFile& other) {
	fileHandle->getContent().merge(other);
	for (auto& [otherSectionName, otherSection] : other) {
		auto& thisSection = fileHandle->getContent()[otherSectionName];
		thisSection.merge(otherSection);
	}
}

} // namespace tactics::resource
