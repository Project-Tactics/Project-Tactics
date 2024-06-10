#pragma once

#include <Libs/Ecs/Entity.h>

#include <nlohmann/json.hpp>

namespace tactics {
namespace resource {
class ResourceProvider;
}

template <typename T>
concept HasDeserialize = requires(T obj, resource::ResourceProvider * provider, const nlohmann::ordered_json & json) {
	{ obj.deserialize(provider, json) };
};

template<typename ...TArgs>
void defineComponentsReflection() {
	(TArgs::defineReflection(), ...);
}

template<typename TComponent>
entt::meta_factory<TComponent> componentReflection(const char* name) {
	auto metaFactory = entt::meta<TComponent>()
		.type(hash(name))
		.func<&emplaceComponent<TComponent>>(hash("emplace"))
		.func<&cloneComponent<TComponent>>(hash("clone"));

	if constexpr (HasDeserialize<TComponent>) {
		metaFactory.func<&TComponent::deserialize>(hash("deserializer"));
	}
	return metaFactory;
}

}
