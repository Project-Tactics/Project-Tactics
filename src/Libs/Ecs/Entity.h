#pragma once

#include "EntityUtilities.h"

#include <Libs/Utility/Exception.h>
#include <Libs/Utility/HashId.h>

namespace tactics {

class EntityComponentSystem;

class Entity {
private:
	Entity(const HashId& name, entt::registry* registry);

public:
	Entity();

	static Entity create(const HashId& name, entt::registry* registry);
	static Entity create(entt::entity entity, entt::registry* registry);
	void destroy();

	const HashId& getName() const;

	template<typename Type> decltype(auto) getComponent();

	template<typename Type> decltype(auto) getComponent() const;

	template<typename Type, typename... Func> decltype(auto) updateComponent(Func&&... func);

	template<typename Type> bool hasComponent() const;

	template<typename Type, typename... TArgs> decltype(auto) addComponent(TArgs&&... args);

	template<typename Type> decltype(auto) addComponent(Type& component);

	template<typename Type> decltype(auto) removeComponent();

	bool operator==(entt::entity entity) const;
	explicit operator bool() const;

private:
	entt::entity _entity;
	entt::registry* _registry{};
};

} // namespace tactics

#include "Entity.inl"
