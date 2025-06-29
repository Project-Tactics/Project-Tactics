#pragma once

#include "../Resource.h"

namespace tactics::resource {

class BaseDataSet : public Resource<BaseDataSet> {
public:
	using Resource<BaseDataSet>::Resource;
	static const ResourceType TYPE = ResourceType::DataSet;
};

template<typename T> class DataSet : public BaseDataSet {
public:
	using BaseDataSet::BaseDataSet;
	std::vector<T> data;
};

} // namespace tactics::resource
