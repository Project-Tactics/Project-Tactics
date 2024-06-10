#include "PrefabManager.h"

#include "Component/PrefabComponent.h"

#include <Libs/Resource/ResourceProvider.h>

namespace tactics {

Entity PrefabManager::clonePrefabToRegistry(const std::string& name, const Entity& prefabEntity, entt::registry& destRegistry) {
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

Entity PrefabManager::createPrefab(const std::string& name, const nlohmann::ordered_json& json, const resource::ResourceProvider& resourceProvider) {
	auto entity = Entity::create(name, &_registry);
	auto& prefab = entity.addComponent<component::Prefab>();

	for (auto& [key, value] : json.items()) {
		auto id = hash(key);
		auto type = entt::resolve(id);

		if (!type) {
			throw TACTICS_EXCEPTION("Component type not found while loading prefab: [{}]", key);
		}

		prefab.componentTypes.push_back(id);
		auto componentInstance = type.construct();
		_buildComponentRecursively(componentInstance, value, resourceProvider);
		if (auto func = type.func(hash("emplace"))) {
			func.invoke(componentInstance, entity);
		} else {
			throw TACTICS_EXCEPTION("Missing [emplace] function for Component. A component reflection must provide it. Component: [{}]", key);
		}
	}

	return entity;
}

void PrefabManager::_buildComponentRecursively(entt::meta_any& instance, const nlohmann::ordered_json& jsonData, const resource::ResourceProvider& resourceProvider) {
	auto type = instance.type();

	if (auto deserializer = type.func(hash("deserializer"))) {
		deserializer.invoke(instance, &resourceProvider, jsonData);
		return;
	}

	for (auto& [key, value] : jsonData.items()) {
		auto id = hash(key);

		if (auto customDeserializer = type.func(hash(key))) {
			customDeserializer.invoke(instance, value);
		} else {
			auto data = type.data(id);
			if (!data) {
				throw TACTICS_EXCEPTION("Can't find data type while loading prefab: [{}]", key);
			}

			auto memberType = data.type();

			if (memberType.is_integral()) {
				data.set(instance, value.get<int>());
			} else if (memberType.is_arithmetic()) {
				data.set(instance, value.get<float>());
			} else if (memberType.is_enum()) {
				data.set(instance, memberType.data(hash(value.get<std::string>())).get({}));
			} else if (memberType.is_pointer() || memberType.is_pointer_like()) {
				throw TACTICS_EXCEPTION("Pointer types are not automatically deserializable while loading prefabs. Field: [{}]. Consider to use a custom deserializer function instead",
					key);
			} else if (memberType.is_class()) {
				auto memberInstance = data.get(instance);
				_buildComponentRecursively(memberInstance, value, resourceProvider);
				data.set(instance, memberInstance);
			}
		}
	}
}

entt::registry& PrefabManager::getRegistry() {
	return _registry;
}

const entt::registry& PrefabManager::getRegistry() const {
	return _registry;
}

}
