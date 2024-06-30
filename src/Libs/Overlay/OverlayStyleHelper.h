#pragma once

namespace tactics {
namespace resource {
class IniFile;
}

class FileSystem;

class OverlayStyleHelper {
public:
	static void setupImGuiStyle(const resource::IniFile& iniFile, const FileSystem& fileSystem);
};

} // namespace tactics
