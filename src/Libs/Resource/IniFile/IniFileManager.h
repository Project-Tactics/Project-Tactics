#pragma once

#include <Libs/Resource/ResourceManager.h>
#include "IniFile.h"

namespace tactics {

class IniFileManager: public TResourceManager<IniFile> {
public:
	using TResourceManager<IniFile>::TResourceManager;
	std::vector<ResourceId> load(sol::reference& luaDefinitionLoader) override;
	void unload(ResourceId resourceId) override;
	void unload(std::vector<ResourceId> resourceIds) override;
};

}
