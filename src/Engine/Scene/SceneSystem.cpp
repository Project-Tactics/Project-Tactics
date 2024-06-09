#include "SceneSystem.h"

#include <Libs/Ecs/EntityComponentSystem.h>
#include <Libs/Ecs/Component/AlphaBlendedComponent.h>
#include <Libs/Ecs/Component/CameraComponent.h>
#include <Libs/Ecs/Component/FrustumComponent.h>
#include <Libs/Ecs/Component/MeshComponent.h>
#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Ecs/Component/ViewportComponent.h>
#include <Libs/Ecs/System/CameraSystem.h>
#include <Libs/Resource/Material/Material.h>
#include <Libs/Resource/Mesh/Mesh.h>
#include <Libs/Resource/Prefab/Prefab.h>
#include <Libs/Resource/ResourceSystem.h>

#include <ranges>
#include <regex>

namespace tactics {

SceneSystem::SceneSystem(
	EntityComponentSystem& ecs,
	resource::ResourceSystem& resourceSystem)
	: _ecs(ecs)
	, _resourceSystem(resourceSystem) {
	using namespace component;
	_ecs.sceneRegistry().on_construct<Mesh>().connect<&SceneSystem::_onMeshConstructed>(this);
	_ecs.sceneRegistry().on_update<Mesh>().connect<&SceneSystem::_onMeshUpdated>(this);
	_ecs.sceneRegistry().on_construct<CurrentCamera>().connect<&SceneSystem::_onCurrentCameraConstructed>(this);
	_ecs.sceneRegistry().on_construct<CurrentViewport>().connect<&SceneSystem::_onCurrentViewportConstructed>(this);
}

SceneSystem::~SceneSystem() {
}

void SceneSystem::clearScene(bool clearCameras) {
	_ecs.sceneRegistry().view<component::Mesh>().each([this] (auto entity, auto&) {
		_ecs.sceneRegistry().destroy(entity);
	});

	if (clearCameras) {
		_ecs.sceneRegistry().view<component::Camera>().each([this] (auto entity, auto&) {
			_ecs.sceneRegistry().destroy(entity);
		});
	}
}

Entity& SceneSystem::getCurrentCamera() {
	if (!_currentCameraEntity) {
		throw TACTICS_EXCEPTION("Trying to get the current camera but the current entity is not valid.");
	}
	return _currentCameraEntity;
}

void SceneSystem::_onCurrentCameraConstructed(entt::registry&, entt::entity currentCameraEntity) {
	using namespace component;
	_ecs.sceneRegistry().view<CurrentCamera>().each([this, currentCameraEntity] (auto entity) {
		if (currentCameraEntity != entity) {
			_ecs.sceneRegistry().remove<CurrentCamera>(entity);
		}
	});

	_currentCameraEntity = Entity::create(currentCameraEntity, &_ecs.sceneRegistry());
}

void SceneSystem::_onCurrentViewportConstructed(entt::registry&, entt::entity currentViewportEntity) {
	using namespace component;
	_ecs.sceneRegistry().view<CurrentViewport>().each([this, currentViewportEntity] (auto entity) {
		if (currentViewportEntity != entity) {
			_ecs.sceneRegistry().remove<CurrentViewport>(entity);
		}
	});
}

void SceneSystem::_onMeshConstructed(entt::registry& registry, entt::entity entity) {
	_updateAlphaBlendFlags(registry, entity);
}

void SceneSystem::_onMeshUpdated(entt::registry& registry, entt::entity entity) {
	_updateAlphaBlendFlags(registry, entity);
}

void SceneSystem::_updateAlphaBlendFlags(entt::registry& registry, entt::entity entity) {
	using namespace component;

	auto& mesh = registry.get<Mesh>(entity);

	auto isFullyTransparent = true;
	auto isMixedAlphaBlended = false;
	for (auto& material : mesh.materials) {
		isMixedAlphaBlended |= material->parent->hasAlphaBlend;
		isFullyTransparent &= material->parent->hasAlphaBlend;
	}

	registry.remove<AlphaBlended>(entity);
	registry.remove<FullyAlphaBlended>(entity);

	if (isFullyTransparent) {
		registry.emplace<FullyAlphaBlended>(entity);
		registry.emplace<AlphaBlended>(entity);
	} else if (isMixedAlphaBlended) {
		registry.emplace<AlphaBlended>(entity);
	}
}

Entity SceneSystem::createViewport(const glm::vec2& topLeft, const glm::vec2& size, const glm::vec4& clearColor) {
	auto entity = Entity::create("viewport", &_ecs.sceneRegistry());
	entity.addComponent<component::Viewport>(topLeft, size, clearColor);
	return entity;
}

Entity SceneSystem::createCamera(
	std::string_view name,
	const glm::vec3& position,
	const glm::vec3& direction,
	const glm::vec3& up,
	float fov,
	float near,
	float far
) {
	using namespace component;
	auto entity = Entity::create(name, &_ecs.sceneRegistry());
	auto& transform = entity.addComponent<Transform>();
	transform.setPosition(position);
	transform.setRotation(glm::quatLookAt(direction, up));

	entity.addComponent<Frustum>(fov, near, far, 1.f);
	entity.addComponent<Camera>();
	return entity;
}

Entity SceneSystem::createEntity(
	const glm::vec3& position,
	std::string_view meshName,
	std::vector<std::string> materials,
	const glm::quat& rotation,
	const glm::vec3& scale
) {
	using namespace component;
	auto entity = Entity::create("", &_ecs.sceneRegistry());
	auto& transform = entity.addComponent<Transform>();
	transform.setPosition(position);
	transform.setRotation(rotation);
	transform.setScale(scale);

	Mesh meshComp;
	meshComp.mesh = _resourceSystem.getResource<resource::Mesh>(meshName);
	for (auto i = 0; i < meshComp.mesh->subMeshes.size(); ++i) {
		const std::string& materialName = i < materials.size() ? materials[i] : materials.back();
		auto material = _resourceSystem.getResource<resource::Material>(materialName);
		meshComp.materials.push_back(resource::Material::createInstance(material));
	}
	entity.addComponent<Mesh>(meshComp);
	return entity;
}

Entity SceneSystem::createEntity(
	std::string_view name,
	std::string_view prefabName
) {
	auto prefab = _resourceSystem.getResource<resource::Prefab>(prefabName);
	auto entity = _ecs.createEntityFromPrefab(std::string(name), prefab->entity);
	return entity;
}

}
