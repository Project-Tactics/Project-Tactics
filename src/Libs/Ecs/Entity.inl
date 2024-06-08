namespace tactics {

template<typename Type>
decltype(auto) Entity::getComponent() {
	if (hasComponent<Type>()) {
		return _registry->get<Type>(_entity);
	}
	throw TACTICS_EXCEPTION("Can't get component. Entity {} does not have the requested component", toString(getName()));
}

template<typename Type>
decltype(auto) Entity::getComponent() const {
	if (hasComponent<Type>()) {
		return _registry->get<Type>(_entity);
	}
	throw TACTICS_EXCEPTION("Can't get component {}. Entity {} does not have the requested component",
		entt::resolve<Type>().info().name(), toString(getName()));
}

template<typename Type, typename... Func>
decltype(auto) Entity::updateComponent(Func &&...func) {
	if (hasComponent<Type>()) {
		return _registry->patch<Type>(_entity, std::forward<Func>(func)...);
	}
	throw TACTICS_EXCEPTION("Can't update component. Entity {} does not have the requested component", toString(getName()));
}

template<typename Type>
bool Entity::hasComponent() const {
	return _registry->any_of<Type>(_entity);
}

template<typename Type, typename ...TArgs>
decltype(auto) Entity::addComponent(TArgs&&... args) {
	if (hasComponent<Type>()) {
		throw TACTICS_EXCEPTION("Entity {} already has the requested component", toString(getName()));
	}
	return _registry->emplace<Type>(_entity, std::forward<TArgs&&>(args)...);
}

template<typename Type>
decltype(auto) Entity::addComponent(Type& component) {
	if (hasComponent<Type>()) {
		throw TACTICS_EXCEPTION("Entity {} already has the requested component", toString(getName()));
	}
	return _registry->emplace<Type>(_entity, component);
}

template<typename Type>
decltype(auto) Entity::removeComponent() {
	if (!hasComponent<Type>()) {
		throw TACTICS_EXCEPTION("Can't remove component. Entity {} does not have a component of that type", toString(getName()));
	}
	return _registry->remove<Type>(_entity);
}

template<typename Type>
static decltype(auto) Entity::explicitAddComponent(Type& component, Entity& entity) {
	return entity.addComponent<Type>(component);
}

}
