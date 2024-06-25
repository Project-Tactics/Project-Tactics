#include "JsonSerialization.h"

namespace nlohmann {

void adl_serializer<tactics::hash_string>::to_json(json& j, const tactics::hash_string& value) {
	j = value.data();
}

void adl_serializer<tactics::hash_string>::from_json(const json& j, tactics::hash_string& value) {
	value = tactics::hash_string(j.get<std::string>().c_str());
}

}
