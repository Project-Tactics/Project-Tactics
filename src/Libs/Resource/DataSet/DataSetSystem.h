#pragma once

#include "../ResourceSystem.h"
#include "DataSet.h"

namespace tactics::resource {

class ResourceSystem;

/*
 * A small utility class meant to retrieve DataSet resources in an easier way
 */
class DataSetSystem {
public:
	DataSetSystem(ResourceSystem& resourceSystem);

	template<typename TDataSetItem> std::shared_ptr<DataSet<TDataSetItem>> getDataSet(HashId name) {
		return _resourceSystem.getResource<DataSet<TDataSetItem>>(name);
	}

private:
	ResourceSystem& _resourceSystem;
};

} // namespace tactics::resource
