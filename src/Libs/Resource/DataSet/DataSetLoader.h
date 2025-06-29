#pragma once

#include "../ResourceLoader.h"
#include "DataSet.h"

namespace tactics {
namespace resource {

struct DataSetDescriptor {
	std::string path;
	HashId type;

	JSON_SERIALIZE(DataSetDescriptor, path, type);
};

class DataSetLoader : public ResourceLoader {
public:
	DataSetLoader(FileSystem& fileSystem, const ResourceProvider& resourceProvider);
	[[nodiscard]] std::shared_ptr<BaseDataSet> load(const DataSetDescriptor& descriptor);

private:
};

} // namespace resource
} // namespace tactics
