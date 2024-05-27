#pragma once

#include "ResourcePathHelper.h"
#include "ResourceProvider.h"

namespace tactics::resource {

class ResourceLoader {
public:
	ResourceLoader(const ResourcePathHelper& pathHelper, const ResourceProvider& resourceProvider)
		: _pathHelper(pathHelper)
		, _resourceProvider(resourceProvider) {
	}

protected:
	std::string _makeAbsolutePath(const std::string& path) {
		return _pathHelper.makeAbsolutePath(path);
	}

	template<typename TResource>
	std::shared_ptr<TResource> _getResource(std::string_view name) {
		return _resourceProvider.getResource<TResource>(name);
	}

	template<typename TResource>
	std::shared_ptr<TResource> _getResource(ResourceId id) {
		return _resourceProvider.getResource<TResource>(id);
	}

private:
	const ResourcePathHelper& _pathHelper;
	const ResourceProvider& _resourceProvider;
};

}
