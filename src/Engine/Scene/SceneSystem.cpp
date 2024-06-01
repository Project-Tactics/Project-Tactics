#include "SceneSystem.h"


#include <Libs/Ecs/EntityComponentSystem.h>
#include <Libs/Ecs/Component/AlphaBlendedComponent.h>
#include <Libs/Ecs/Component/CameraComponent.h>
#include <Libs/Ecs/Component/FrustumComponent.h>
#include <Libs/Ecs/Component/MeshComponent.h>
#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Ecs/Component/ViewportComponent.h>
#include <Libs/Ecs/SubSystem/CameraSubSystem.h>
#include <Libs/Resource/Material/Material.h>
#include <Libs/Resource/Mesh/Mesh.h>
#include <Libs/Resource/ResourceSystem.h>

#include <ranges>

namespace tactics {

SceneSystem::SceneSystem(
	EntityComponentSystem& ecs,
	resource::ResourceSystem& resourceSystem)
	: _ecs(ecs)
	, _resourceSystem(resourceSystem) {
	using namespace component;
	_ecs.on_construct<Mesh>().connect<&SceneSystem::_onMeshConstructed>(this);
	_ecs.on_update<Mesh>().connect<&SceneSystem::_onMeshUpdated>(this);
	_ecs.on_construct<CurrentCamera>().connect<&SceneSystem::_onCurrentCameraConstructed>(this);
	_ecs.on_construct<CurrentViewport>().connect<&SceneSystem::_onCurrentViewportConstructed>(this);

	_cameraSubSystem = std::make_unique<CameraSubSystem>(_ecs);
	_ecs.registerSubSystem(_cameraSubSystem.get());
}

SceneSystem::~SceneSystem() {
	_ecs.unregisterSubSystem(_cameraSubSystem.get());
}

EntityObject& SceneSystem::getCurrentCamera() {
	if (_currentCameraEntity == entt::null) {
		throw Exception("Trying to get the current camera but the current entity is not valid.");
	}
	return _currentCameraEntity;
}

void SceneSystem::_onCurrentCameraConstructed(entt::registry&, entt::entity currentCameraEntity) {
	using namespace component;
	_ecs.view<CurrentCamera>().each([this, currentCameraEntity] (auto entity) {
		if (currentCameraEntity != entity) {
			_ecs.remove<CurrentCamera>(entity);
		}
	});

	_currentCameraEntity = EntityObject::create(currentCameraEntity, &_ecs);
}

void SceneSystem::_onCurrentViewportConstructed(entt::registry&, entt::entity currentViewportEntity) {
	using namespace component;
	_ecs.view<CurrentViewport>().each([this, currentViewportEntity] (auto entity) {
		if (currentViewportEntity != entity) {
			_ecs.remove<CurrentViewport>(entity);
		}
	});
}

void SceneSystem::_onMeshConstructed(entt::registry& registry, entt::entity entity) {
	_updateAlphaBlended(registry, entity);
}

void SceneSystem::_onMeshUpdated(entt::registry& registry, entt::entity entity) {
	_updateAlphaBlended(registry, entity);
}

void SceneSystem::_updateAlphaBlended(entt::registry& registry, entt::entity entity) {
	using namespace component;

	auto& material = registry.get<Mesh>(entity);
	auto itr = std::ranges::find_if(material.material->getTextures(), [] (auto& pair) { return pair.second->info.useTransparency; });
	if (itr != material.material->getTextures().end()) {
		registry.emplace<AlphaBlended>(entity);
	} else {
		registry.remove<AlphaBlended>(entity);
	}
}

EntityObject SceneSystem::createViewport(const glm::vec2& topLeft, const glm::vec2& size, const glm::vec4& clearColor) {
	auto entity = EntityObject::create("viewport", &_ecs);
	entity.addComponent<component::Viewport>(topLeft, size, clearColor);
	return entity;
}

EntityObject SceneSystem::createCamera(
	const entt::hashed_string& name,
	const glm::vec3& position,
	const glm::vec3& direction,
	const glm::vec3& up,
	float fov,
	float near,
	float far
) {
	using namespace component;
	auto entity = EntityObject::create(name, &_ecs);
	auto& transform = entity.addComponent<Transform>();
	transform.setPosition(position);
	transform.setRotation(glm::quatLookAt(direction, up));

	entity.addComponent<Frustum>(fov, near, far, 1.f);
	entity.addComponent<Camera>();
	return entity;
}

EntityObject SceneSystem::createEntity(
		const glm::vec3& position,
		std::string_view meshName,
		std::string_view materialName,
		const glm::quat& rotation,
		const glm::vec3& scale
) {
	using namespace component;
	auto entity = EntityObject::create("", &_ecs);
	auto& transform = entity.addComponent<Transform>();
	transform.setPosition(position);
	transform.setRotation(rotation);
	transform.setScale(scale);

	Mesh meshComp;
	auto material = _resourceSystem.getResource<resource::Material>(materialName);
	meshComp.material = resource::Material::createInstance(material);
	meshComp.mesh = _resourceSystem.getResource<resource::Mesh>(meshName);
	entity.addComponent<Mesh>(meshComp);
	return entity;
}

}
