#pragma once

#include <Libs/Resource/ResourceLoader.h>

#include "IniFile.h"

namespace tactics::resource {

struct IniFileLoadDescriptor {
	std::string name;
	std::string path;
	std::string pathToDefault;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(IniFileLoadDescriptor, name, path, pathToDefault);
};

class IniFileLoader: public ResourceLoader {
public:
	std::unique_ptr<IniFile> load(const IniFileLoadDescriptor& descriptor);
};

}
