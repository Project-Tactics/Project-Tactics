#include "IniFileLoader.h"

#include <Libs/Resource/ResourcePathHelper.h>

#include <filesystem>

namespace tactics::resource {

std::shared_ptr<IniFile> IniFileLoader::load(const IniFileLoadDescriptor& descriptor) {
	auto path = _makeAbsolutePath(descriptor.path);
	auto defaultPath = _makeAbsolutePath(descriptor.pathToDefault);

	auto iniFile = std::make_unique<IniFile>(descriptor.name);
	iniFile->filename = path;
	iniFile->saveOnUnload = descriptor.saveOnUnload;
	if (std::filesystem::exists(path)) {
		iniFile->file.load(path);
	} else {
		iniFile->file.save(path);
	}

	std::ifstream defaultIniStream(defaultPath);
	ini::IniFile file(defaultIniStream);
	iniFile->file.merge(file);
	return iniFile;
}

}
