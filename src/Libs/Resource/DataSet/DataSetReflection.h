#pragma once

#include <Libs/Resource/DataSet/DataSet.h>
#include <Libs/Utility/Reflection.h>

#define DATASET(TYPE, ...)                       \
	JSON_SERIALIZE(TYPE, __VA_ARGS__);           \
	static void defineReflection() {             \
		tactics::dataSetReflection<TYPE>(#TYPE); \
	}

namespace tactics {

template<typename TDataSetItem>
tactics::resource::BaseDataSet::Ptr dataSetJsonDeserializer(const nlohmann::ordered_json& json) {
	using namespace tactics::resource;
	auto dataSet = std::make_shared<DataSet<TDataSetItem>>();
	if (json.is_array()) {
		dataSet->data.reserve(json.size());
		for (const auto& item : json) {
			dataSet->data.push_back({});
			from_json(item, dataSet->data.back());
		}
	}
	return dataSet;
}

template<typename TDataSetItem> void dataSetReflection(const char* name) {
	using namespace tactics::resource;
	auto meta = entt::meta<DataSet<TDataSetItem>>()
					.type(HashId(name))
					.func<&dataSetJsonDeserializer<TDataSetItem>>("deserialize"_id);
}

} // namespace tactics
