#pragma once

#include <Libs/Resource/ResourceLoader.h>

#include "IniFile.h"

namespace tactics::resource {

struct IniFileLoadDescriptor {
	std::string path;
	std::string pathToDefault;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(IniFileLoadDescriptor, path, pathToDefault);
};

class IniFileLoader: public ResourceLoader {
public:
	using ResourceLoader::ResourceLoader;
	[[nodiscard]] std::shared_ptr<IniFile> load(const IniFileLoadDescriptor& descriptor);
};

}
