#include <Libs/Resource/ResourceProvider.h>

#include "../HashString.h"

namespace nlohmann {
template<>
struct adl_serializer<tactics::hash_string> {
	static void to_json(json& j, const tactics::hash_string& value);
	static void from_json(const json& j, tactics::hash_string& value);
};
}

namespace tactics {

template<typename TComponent>
void defaultJsonDeserializer(TComponent& component, const resource::ResourceProvider* /*resourceProvider*/, const nlohmann::ordered_json& json) {
	if constexpr (sizeof(TComponent) == 1u) {
		// This is a struct with no members, so we don't need to do anything
	} else {
		from_json(json, component);
	}
}

}
