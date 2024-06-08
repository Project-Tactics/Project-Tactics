#include "Entity.h"

#include "Component/NameComponent.h"

namespace tactics {

Entity::Entity(std::string_view name, entt::registry* registry): _registry(registry) {
	_entity = registry->create();
	_registry->emplace<component::Name>(_entity, hash(name));
}

Entity::Entity() {
	_entity = entt::null;
}

const hash_string& Entity::getName() const {
	return getComponent<component::Name>().name;
}

Entity Entity::create(std::string_view name, entt::registry* registry) {
	return Entity(name, registry);
}

Entity Entity::create(entt::entity entity, entt::registry* registry) {
	Entity entityObject;
	entityObject._entity = entity;
	entityObject._registry = registry;
	return entityObject;
}

bool Entity::operator==(entt::entity entity) const {
	return _entity == entity;
}

Entity::operator bool() const {
	return _entity != entt::null;
}

}
