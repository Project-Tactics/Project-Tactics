#pragma once

namespace tactics {
namespace resource {
class ResourcePathHelper;
class IniFile;
}

class OverlayStyleHelper {
public:
	static void setupImGuiStyle(const resource::IniFile& iniFile, const resource::ResourcePathHelper& resourcePathHelper);
};

}
