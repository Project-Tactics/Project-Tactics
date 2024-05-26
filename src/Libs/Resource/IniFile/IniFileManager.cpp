#include "IniFileManager.h"

#include <Libs/Resource/ResourcePathHelper.h>

#include <filesystem>

namespace tactics::resource {

struct IniFileLoadDescriptor {
	std::string name;
	std::string path;
	std::string pathToDefault;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(IniFileLoadDescriptor, name, path, pathToDefault);
};

ResourceId IniFileManager::load(const nlohmann::json& descriptor) {
	auto iniFileDescriptor = descriptor.template get<IniFileLoadDescriptor>();

	auto path = _pathHelper.makeAbsolutePath(iniFileDescriptor.path);
	auto defaultPath = _pathHelper.makeAbsolutePath(iniFileDescriptor.pathToDefault);

	auto iniFile = std::make_unique<IniFile>(iniFileDescriptor.name);
	iniFile->filename = path;
	if (std::filesystem::exists(path)) {
		iniFile->file.load(path);
	} else {
		iniFile->file.save(path);
	}

	std::ifstream defaultIniStream(defaultPath);
	ini::IniFile file(defaultIniStream);
	iniFile->file.merge(file);

	auto id = iniFile->id;
	_registerResource(std::move(iniFile));
	return id;
}

}
