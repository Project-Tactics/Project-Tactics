#pragma once

#include <Libs/Resource/ResourceManager.h>

#include "IniFile.h"

namespace tactics::resource {

class IniFileManager: public TResourceManager<IniFile> {
public:
	using TResourceManager<IniFile>::TResourceManager;
	ResourceId load(const nlohmann::json& descriptor) override;
};

}
