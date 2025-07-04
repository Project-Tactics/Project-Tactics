#include "IniFileLoader.h"

#include <Libs/FileSystem/FileSystem.h>
#include <Libs/Utility/Exception.h>

namespace tactics::resource {

std::shared_ptr<IniFile> IniFileLoader::load(const IniFileLoadDescriptor& descriptor) {
	auto& fileSystem = _getFileSystem();

	auto iniFile = std::make_unique<IniFile>();
	auto iniFileHandle = fileSystem.createIniFileHandle(descriptor.path);
	if (iniFileHandle->exists()) {
		iniFileHandle->load();
	} else {
		iniFileHandle->save();
	}

	iniFile->fileHandle = std::move(iniFileHandle);

	if (!descriptor.pathToDefault.empty()) {
		auto defaultIniHandle = fileSystem.createIniFileHandle(descriptor.pathToDefault);
		if (defaultIniHandle->exists()) {
			defaultIniHandle->load();
			iniFile->merge(defaultIniHandle->getContent());
		} else {
			TACTICS_EXCEPTION("A default ini has been specified but could not be found: {}",
									descriptor.pathToDefault);
		}
	}

	return iniFile;
}

} // namespace tactics::resource
