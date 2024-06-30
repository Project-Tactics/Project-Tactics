#pragma once

#include <Libs/FileSystem/FileHandle/FileHandle.h>
#include <Libs/Resource/Resource.h>

#include <inicpp.h>

namespace tactics::resource {

class IniFile : public Resource<IniFile> {
public:
	using Resource<IniFile>::Resource;
	static const ResourceType TYPE = ResourceType::IniFile;

	ini::IniSection& createSection(std::string_view section);

	template<typename T> T getOrCreate(std::string_view sectionName, std::string_view key, T defaultValue) {
		auto& section = createSection(sectionName);
		auto itr = section.find(key.data());
		if (itr == section.end()) {
			ini::IniField field;
			field = defaultValue;
			section.insert({key.data(), field});
			return defaultValue;
		}
		return itr->second.as<T>();
	}

	template<typename T> T get(std::string_view sectionName, std::string_view key, T defaultValue) {
		auto& iniFile = fileHandle->getContent();
		if (!iniFile.contains(sectionName.data())) { return defaultValue; }

		auto& section = iniFile[std::string(sectionName)];
		auto itr = section.find(key.data());
		if (itr == section.end()) { return defaultValue; }

		return itr->second.as<T>();
	}

	template<typename T> void set(std::string_view sectionName, std::string_view key, T&& value) {
		auto& iniFile = fileHandle->getContent();
		iniFile[sectionName.data()][key.data()] = value;
	}

	void merge(ini::IniFile& other);

	std::unique_ptr<FileHandle<ini::IniFile>> fileHandle;
};

} // namespace tactics::resource
