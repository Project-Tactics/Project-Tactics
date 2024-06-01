#include "Entity.h"

namespace tactics {

EntityObject::EntityObject(const char* name, EntityComponentSystem* ecs): _ecs(ecs) {
	_name = hash_string(name);
	_entity = ecs->create();
}

EntityObject::EntityObject() {
	_entity = entt::null;
	_name = hash_string("NoName");
}

EntityObject EntityObject::create(const char* name, EntityComponentSystem* ecs) {
	return EntityObject(name, ecs);
}

EntityObject EntityObject::create(entt::entity entity, EntityComponentSystem* ecs) {
	EntityObject entityObject;
	entityObject._entity = entity;
	entityObject._ecs = ecs;
	return entityObject;
}

void EntityObject::replaceEntity(entt::entity entity) {
	_entity = entity;
}

}
