#include "ResourceLoader.h"

namespace tactics::resource {

ResourceLoader::ResourceLoader(FileSystem& fileSystem, const ResourceProvider& resourceProvider)
	: _fileSystem(fileSystem)
	, _resourceProvider(resourceProvider) {
}

FileSystem& ResourceLoader::_getFileSystem() const {
	return _fileSystem;
}

}
