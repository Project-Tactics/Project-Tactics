#pragma once

#include <Libs/Resource/Resource.h>
#include <inicpp.h>

namespace tactics::resource {

class IniFile: public Resource<IniFile> {
public:
	~IniFile();
	void save();
	void reload();
	ini::IniSection& createSection(std::string_view section);

	template<typename T>
	T getOrCreate(std::string_view sectionName, std::string_view key, T defaultValue) {
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

	template<typename T>
	void set(std::string_view sectionName, std::string_view key, T&& value) {
		file[sectionName.data()][key.data()] = value;
	}

	using Resource<IniFile>::Resource;
	static const ResourceType TYPE = ResourceType::IniFile;
	ini::IniFile file;
	std::string filename;
};

}
