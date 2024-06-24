#pragma once

#include <Libs/Ecs/Entity.h>
#include "Json/MathJsonSerializer.h"
#include "Json/JsonSerialization.h"

#include <nlohmann/json.hpp>

namespace tactics {

template <typename T>
concept HasDeserialize = requires(T obj, resource::ResourceProvider * provider, const nlohmann::ordered_json & json) {
	{ obj.deserialize(provider, json) };
};

template<typename ...TArgs>
void defineComponentsReflection() {
	(TArgs::defineReflection(), ...);
}

template<typename TComponent, bool DefaultDeserialization = true>
entt::meta_factory<TComponent> componentReflection(const char* name) {
	auto metaFactory = entt::meta<TComponent>()
		.type(hash(name))
		.func<&emplaceComponent<TComponent>>(hash("emplace"))
		.func<&cloneComponent<TComponent>>(hash("clone"));


	if constexpr (HasDeserialize<TComponent>) {
		metaFactory.func<&TComponent::deserialize>(hash("deserialize"));
	} else if constexpr (DefaultDeserialization) {
		metaFactory.func<&defaultJsonDeserializer<TComponent>>(hash("deserialize"));
	}
	return metaFactory;
}

}
