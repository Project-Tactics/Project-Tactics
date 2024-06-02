#include "Entity.h"

#include "Component/NameComponent.h"

namespace tactics {

Entity::Entity(const char* name, EntityComponentSystem* ecs): _ecs(ecs) {
	_entity = ecs->create();
	ecs->emplace<component::Name>(_entity, hash_string(name));
}

Entity::Entity() {
	_entity = entt::null;
}

const hash_string& Entity::getName() const {
	return getComponent<component::Name>().name;
}

Entity Entity::create(const char* name, EntityComponentSystem* ecs) {
	return Entity(name, ecs);
}

Entity Entity::create(entt::entity entity, EntityComponentSystem* ecs) {
	Entity entityObject;
	entityObject._entity = entity;
	entityObject._ecs = ecs;
	return entityObject;
}

bool Entity::operator==(entt::entity entity) const {
	return _entity == entity;
}

Entity::operator bool() const {
	return _entity != entt::null;
}

}
