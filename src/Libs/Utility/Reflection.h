#pragma once

#include "Json/JsonSerialization.h"

#include <Libs/Ecs/Entity.h>
#include <Libs/Resource/ResourceProvider.h>
#include <Libs/Resource/ResourceSerialization.h>
#include <Libs/Utility/String/String.h>

namespace tactics {

template<typename TComponent>
void defaultJsonDeserializer(TComponent& component,
							 const resource::ResourceProvider* /*resourceProvider*/,
							 const nlohmann::ordered_json& json) {
	if constexpr (sizeof(TComponent) == 1u) {
		// This is a struct with no members, so we don't need to do anything
	} else {
		from_json(json, component);
	}
}

template<typename T>
concept HasDeserialize = requires(T obj, resource::ResourceProvider* provider, const ordered_json& json) {
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

#define REFLECT1(TYPE, MEMBER)		 .data<&TYPE::MEMBER>(HashId(#MEMBER))
#define REFLECT2(TYPE, MEMBER, ...)	 REFLECT1(TYPE, MEMBER) REFLECT1(TYPE, __VA_ARGS__)
#define REFLECT3(TYPE, MEMBER, ...)	 REFLECT1(TYPE, MEMBER) REFLECT2(TYPE, __VA_ARGS__)
#define REFLECT4(TYPE, MEMBER, ...)	 REFLECT1(TYPE, MEMBER) REFLECT3(TYPE, __VA_ARGS__)
#define REFLECT5(TYPE, MEMBER, ...)	 REFLECT1(TYPE, MEMBER) REFLECT4(TYPE, __VA_ARGS__)
#define REFLECT6(TYPE, MEMBER, ...)	 REFLECT1(TYPE, MEMBER) REFLECT5(TYPE, __VA_ARGS__)
#define REFLECT7(TYPE, MEMBER, ...)	 REFLECT1(TYPE, MEMBER) REFLECT6(TYPE, __VA_ARGS__)
#define REFLECT8(TYPE, MEMBER, ...)	 REFLECT1(TYPE, MEMBER) REFLECT7(TYPE, __VA_ARGS__)
#define REFLECT9(TYPE, MEMBER, ...)	 REFLECT1(TYPE, MEMBER) REFLECT8(TYPE, __VA_ARGS__)
#define REFLECT10(TYPE, MEMBER, ...) REFLECT1(TYPE, MEMBER) REFLECT9(TYPE, __VA_ARGS__)
#define REFLECT11(TYPE, MEMBER, ...) REFLECT1(TYPE, MEMBER) REFLECT10(TYPE, __VA_ARGS__)
#define REFLECT12(TYPE, MEMBER, ...) REFLECT1(TYPE, MEMBER) REFLECT11(TYPE, __VA_ARGS__)
#define REFLECT13(TYPE, MEMBER, ...) REFLECT1(TYPE, MEMBER) REFLECT12(TYPE, __VA_ARGS__)
#define REFLECT14(TYPE, MEMBER, ...) REFLECT1(TYPE, MEMBER) REFLECT13(TYPE, __VA_ARGS__)
#define REFLECT15(TYPE, MEMBER, ...) REFLECT1(TYPE, MEMBER) REFLECT14(TYPE, __VA_ARGS__)
#define REFLECT16(TYPE, MEMBER, ...) REFLECT1(TYPE, MEMBER) REFLECT15(TYPE, __VA_ARGS__)
#define REFLECT17(TYPE, MEMBER, ...) REFLECT1(TYPE, MEMBER) REFLECT16(TYPE, __VA_ARGS__)
#define REFLECT18(TYPE, MEMBER, ...) REFLECT1(TYPE, MEMBER) REFLECT17(TYPE, __VA_ARGS__)
#define REFLECT19(TYPE, MEMBER, ...) REFLECT1(TYPE, MEMBER) REFLECT18(TYPE, __VA_ARGS__)
#define REFLECT20(TYPE, MEMBER, ...) REFLECT1(TYPE, MEMBER) REFLECT19(TYPE, __VA_ARGS__)

#define GET_MACRO(_1,   \
				  _2,   \
				  _3,   \
				  _4,   \
				  _5,   \
				  _6,   \
				  _7,   \
				  _8,   \
				  _9,   \
				  _10,  \
				  _11,  \
				  _12,  \
				  _13,  \
				  _14,  \
				  _15,  \
				  _16,  \
				  _17,  \
				  _18,  \
				  _19,  \
				  _20,  \
				  NAME, \
				  ...)  \
	NAME

#define REFLECT_EMPTY(TYPE)               \
	static void defineReflection() {      \
		componentReflection<TYPE>(#TYPE); \
	}

#define REFLECT(TYPE, ...)                                                       \
	JSON_SERIALIZE(TYPE, __VA_ARGS__);                                           \
                                                                                 \
	static void defineReflection() {                                             \
		componentReflection<TYPE>(#TYPE) GET_MACRO(__VA_ARGS__,                  \
												   REFLECT20,                    \
												   REFLECT19,                    \
												   REFLECT18,                    \
												   REFLECT17,                    \
												   REFLECT16,                    \
												   REFLECT15,                    \
												   REFLECT14,                    \
												   REFLECT13,                    \
												   REFLECT12,                    \
												   REFLECT11,                    \
												   REFLECT10,                    \
												   REFLECT9,                     \
												   REFLECT8,                     \
												   REFLECT7,                     \
												   REFLECT6,                     \
												   REFLECT5,                     \
												   REFLECT4,                     \
												   REFLECT3,                     \
												   REFLECT2,                     \
												   REFLECT1)(TYPE, __VA_ARGS__); \
	}
