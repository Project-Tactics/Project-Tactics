#include "JsonSerialization.h"

namespace nlohmann {

void adl_serializer<tactics::HashId>::to_json(json& j, const tactics::HashId& value) {
	j = value.str();
}

void adl_serializer<tactics::HashId>::from_json(const json& j, tactics::HashId& value) {
	value = tactics::HashId(j.get<std::string>().c_str());
}

} // namespace nlohmann
