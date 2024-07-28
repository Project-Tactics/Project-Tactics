#pragma once

#include "IniFile.h"

#include <Libs/Resource/ResourceLoader.h>

namespace tactics::resource {

struct IniFileLoadDescriptor {
	std::string path;
	std::string pathToDefault;

	JSON_SERIALIZE(IniFileLoadDescriptor, path, pathToDefault);
};

class IniFileLoader : public ResourceLoader {
public:
	using ResourceLoader::ResourceLoader;
	[[nodiscard]] std::shared_ptr<IniFile> load(const IniFileLoadDescriptor& descriptor);
};

} // namespace tactics::resource
