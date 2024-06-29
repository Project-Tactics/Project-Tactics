#include <Libs/Resource/ResourceProvider.h>

#include "../HashId.h"

namespace nlohmann {
template<>
struct adl_serializer<tactics::HashId> {
	static void to_json(json& j, const tactics::HashId& value);
	static void from_json(const json& j, tactics::HashId& value);
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
