#include "DataSetLoader.h"

#include <Libs/Ecs/EntityUtilities.h>
#include <Libs/FileSystem/FileSystem.h>

namespace tactics::resource {

DataSetLoader::DataSetLoader(FileSystem& fileSystem, const ResourceProvider& resourceProvider)
	: ResourceLoader(fileSystem, resourceProvider) {}

std::shared_ptr<BaseDataSet> DataSetLoader::load(const DataSetDescriptor& descriptor) {
	auto& fileSystem = _getFileSystem();
	auto jsonHandle = fileSystem.createJsonFileHandle(descriptor.path);
	jsonHandle->load();

	auto type = entt::resolve(descriptor.type);

	if (!type) {
		TACTICS_EXCEPTION("DataSet type not found while loading dataset: [{}]", descriptor.type);
	}

	auto deserializer = type.func("deserialize"_id);
	if (!deserializer) {
		TACTICS_EXCEPTION(
			"Missing [deserialize] function for DataSet. A dataset reflection must be provided. DataSet: [{}]",
			descriptor.type);
	}

	entt::meta_any result = deserializer.invoke({}, jsonHandle->getContent());
	if (!result) {
		// throw an exception by indicating that the result is not valid
		TACTICS_EXCEPTION("Deserialize function for DataSet does not return a valid DataSet. DataSet: [{}]",
						  descriptor.type);
	}

	return result.cast<BaseDataSet::Ptr>();
}

} // namespace tactics::resource
