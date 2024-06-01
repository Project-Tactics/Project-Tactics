#include "Entity.h"

namespace tactics {

Entity::Entity(const char* name, EntityComponentSystem* ecs): _ecs(ecs) {
	_name = hash_string(name);
	_entity = ecs->create();
}

Entity::Entity() {
	_entity = entt::null;
	_name = hash_string("NoName");
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

void Entity::replaceEntity(entt::entity entity) {
	_entity = entity;
}

}
