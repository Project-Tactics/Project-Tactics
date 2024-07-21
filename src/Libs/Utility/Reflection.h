#pragma once

#include "Json/JsonSerialization.h"
#include "Json/MathJsonSerializer.h"

#include <Libs/Ecs/Entity.h>

#include <nlohmann/json.hpp>

namespace tactics {

template<typename T>
concept HasDeserialize = requires(T obj, resource::ResourceProvider* provider, const nlohmann::ordered_json& json) {
	{ obj.deserialize(provider, json) };
};

template<typename... TArgs> void defineComponentsReflection() {
	(TArgs::defineReflection(), ...);
}

template<typename TComponent, bool DefaultDeserialization = true>
entt::meta_factory<TComponent> componentReflection(const char* name) {
	auto metaFactory = entt::meta<TComponent>()
						   .type(HashId(name))
						   .func<&emplaceComponent<TComponent>>("emplace"_id)
						   .func<&cloneComponent<TComponent>>("clone"_id);

	if constexpr (HasDeserialize<TComponent>) {
		metaFactory.func<&TComponent::deserialize>("deserialize"_id);
	} else if constexpr (DefaultDeserialization) {
		metaFactory.func<&defaultJsonDeserializer<TComponent>>("deserialize"_id);
	}
	return metaFactory;
}

} // namespace tactics
