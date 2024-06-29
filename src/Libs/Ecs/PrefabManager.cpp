#include "PrefabManager.h"

#include "Component/PrefabComponent.h"

#include <Libs/Resource/ResourceProvider.h>

namespace tactics {

Entity PrefabManager::clonePrefabToRegistry(const hash_string& name, const Entity& prefabEntity, entt::registry& destRegistry) {
	auto entity = Entity::create(name, &destRegistry);

	auto& prefabComponent = prefabEntity.getComponent<component::Prefab>();
	for (auto&& componentType : prefabComponent.componentTypes) {
		auto type = entt::resolve(componentType);
		if (!type) {
			throw TACTICS_EXCEPTION("Can't create entity [{}]. Component type [{}] not found while cloning: [{}]",
				name, toString(componentType), toString(prefabEntity.getName()));
		}

		if (auto cloneFunction = type.func(hash("clone"))) {
			cloneFunction.invoke({}, prefabEntity, entity);
		} else {
			throw TACTICS_EXCEPTION("Can't create entity [{}]. Missing [clone] function for Component. A component reflection must provide it. Component: [{}]",
				name, toString(componentType));
		}
	}

	return entity;
}

Entity PrefabManager::createPrefab(const hash_string& name, const nlohmann::ordered_json& json, const resource::ResourceProvider& resourceProvider) {
	auto entity = Entity::create(name, &_registry);
	auto& prefab = entity.addComponent<component::Prefab>();

	for (auto& [key, jsonValue] : json.items()) {
		auto id = hash(key);
		auto type = entt::resolve(id);

		if (!type) {
			throw TACTICS_EXCEPTION("Component type not found while loading prefab: [{}]", key);
		}

		prefab.componentTypes.push_back(id);
		auto componentInstance = type.construct();

		if (auto deserializer = type.func(hash("deserialize"))) {
			deserializer.invoke(componentInstance, &resourceProvider, jsonValue);
		} else {
			throw TACTICS_EXCEPTION("Missing [deserialize] function for Component. A component reflection must provide it. Component: [{}]", key);
		}

		if (auto func = type.func(hash("emplace"))) {
			func.invoke(componentInstance, entity);
		} else {
			throw TACTICS_EXCEPTION("Missing [emplace] function for Component. A component reflection must provide it. Component: [{}]", key);
		}
	}

	return entity;
}

entt::registry& PrefabManager::getRegistry() {
	return _registry;
}

const entt::registry& PrefabManager::getRegistry() const {
	return _registry;
}

}
