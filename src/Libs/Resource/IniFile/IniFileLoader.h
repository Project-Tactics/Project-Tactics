#pragma once

#include <Libs/Resource/ResourceLoader.h>

#include "IniFile.h"

namespace tactics::resource {

struct IniFileLoadDescriptor {
	std::string name;
	std::string path;
	std::string pathToDefault;
	bool saveOnUnload = false;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(IniFileLoadDescriptor, name, path, pathToDefault, saveOnUnload);
};

class IniFileLoader: public ResourceLoader {
public:
	using ResourceLoader::ResourceLoader;
	std::shared_ptr<IniFile> load(const IniFileLoadDescriptor& descriptor);
};

}
