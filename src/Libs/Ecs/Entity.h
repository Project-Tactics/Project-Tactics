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

	void replaceEntity(entt::entity entity);

	const hash_string& getName() const {
		return _name;
	}

	template<typename Type>
	decltype(auto) getComponent() {
		if (hasComponent<Type>()) {
			return _ecs->get<Type>(_entity);
		}
		throw Exception("Can't get component. Entity {} does not have the requested component", toString(_name));
	}

	template<typename Type, typename... Func>
	decltype(auto) updateComponent(Func &&...func) {
		if (hasComponent<Type>()) {
			return _ecs->patch<Type>(_entity, std::forward<Func>(func)...);
		}
		throw Exception("Can't update component. Entity {} does not have the requested component", toString(_name));
	}

	template<typename Type>
	bool hasComponent() {
		return _ecs->any_of<Type>(_entity);
	}

	template<typename Type, typename ...TArgs>
	decltype(auto) addComponent(TArgs&&... args) {
		if (hasComponent<Type>()) {
			throw Exception("Entity {} already has the requested component", toString(_name));
		}
		return _ecs->emplace<Type>(_entity, std::forward<TArgs&&>(args)...);
	}

	template<typename Type>
	decltype(auto) removeComponent() {
		if (!hasComponent<Type>()) {
			throw Exception("Can't remove component. Entity {} does not have a component of that type", toString(_name));
		}
		return _ecs->remove<Type>(_entity);
	}

	bool operator==(entt::entity entity) const {
		return _entity == entity;
	}

	bool operator==(entt::null_t nullEntity) const {
		return _entity == nullEntity;
	}

private:
	hash_string _name;
	entt::entity _entity;

	// TODO(Gerark) It bugs me a lot that I have to pass the ECS to the Entity class
	// Might be worth to consider a different approach in the future
	EntityComponentSystem* _ecs{};
};

}
