#pragma once

#include <Libs/Ecs/Entity.h>

namespace tactics {

template<typename ...TArgs>
void defineComponentsReflection() {
	(TArgs::defineReflection(), ...);
}

template<typename TComponent>
entt::meta_factory<TComponent> componentReflection(const char* name) {
	return entt::meta<TComponent>()
		.type(hash(name))
		.func<&emplaceComponent<TComponent>>(hash("emplace"))
		.func<&cloneComponent<TComponent>>(hash("clone"));
}

}
