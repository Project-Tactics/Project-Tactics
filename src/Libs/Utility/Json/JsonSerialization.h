#include <Libs/Resource/ResourceProvider.h>

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
