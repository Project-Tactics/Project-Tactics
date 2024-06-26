#pragma once

#include "ResourceProvider.h"

namespace tactics {
class FileSystem;
}

namespace tactics::resource {

class ResourceLoader {
public:
	ResourceLoader(FileSystem& fileSystem, const ResourceProvider& resourceProvider);

protected:
	FileSystem& _getFileSystem() const;
	const ResourceProvider& _getResourceProvider() const;

	template<typename TResource> std::shared_ptr<TResource> _getResource(const HashId& name) {
		return _resourceProvider.getResource<TResource>(name);
	}

	template<typename TResource> std::shared_ptr<TResource> _getResource(ResourceId id) {
		return _resourceProvider.getResource<TResource>(id);
	}

private:
	FileSystem& _fileSystem;
	const ResourceProvider& _resourceProvider;
};

} // namespace tactics::resource
