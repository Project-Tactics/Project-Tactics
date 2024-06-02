#pragma once

#include "EntityComponentSystem.h"
#include "EntityUtilities.h"

#include <Libs/Utility/Exception.h>

namespace tactics {

class EntityComponentSystem;

class Entity {
private:
	Entity(const char* name, EntityComponentSystem* ecs);

public:
	Entity();

	static Entity create(const char* name, EntityComponentSystem* ecs);
	static Entity create(entt::entity entity, EntityComponentSystem* ecs);

	const hash_string& getName() const;

	template<typename Type>
	decltype(auto) getComponent() {
		if (hasComponent<Type>()) {
			return _ecs->get<Type>(_entity);
		}
		throw TACTICS_EXCEPTION("Can't get component. Entity {} does not have the requested component", toString(getName()));
	}

	template<typename Type>
	decltype(auto) getComponent() const {
		if (hasComponent<Type>()) {
			return _ecs->get<Type>(_entity);
		}
		throw TACTICS_EXCEPTION("Can't get component. Entity {} does not have the requested component", toString(getName()));
	}

	template<typename Type, typename... Func>
	decltype(auto) updateComponent(Func &&...func) {
		if (hasComponent<Type>()) {
			return _ecs->patch<Type>(_entity, std::forward<Func>(func)...);
		}
		throw TACTICS_EXCEPTION("Can't update component. Entity {} does not have the requested component", toString(getName()));
	}

	template<typename Type>
	bool hasComponent() const {
		return _ecs->any_of<Type>(_entity);
	}

	template<typename Type, typename ...TArgs>
	decltype(auto) addComponent(TArgs&&... args) {
		if (hasComponent<Type>()) {
			throw TACTICS_EXCEPTION("Entity {} already has the requested component", toString(getName()));
		}
		return _ecs->emplace<Type>(_entity, std::forward<TArgs&&>(args)...);
	}

	template<typename Type>
	decltype(auto) removeComponent() {
		if (!hasComponent<Type>()) {
			throw TACTICS_EXCEPTION("Can't remove component. Entity {} does not have a component of that type", toString(getName()));
		}
		return _ecs->remove<Type>(_entity);
	}

	bool operator==(entt::entity entity) const;
	explicit operator bool() const;

private:
	entt::entity _entity;

	// TODO(Gerark) It bugs me a lot that I have to pass the ECS to the Entity class
	// Might be worth to look again at the design later. If then there's no better way, then we can remove the TODO
	EntityComponentSystem* _ecs{};
};

}
