#include "SceneSystem.h"

#include <Libs/Ecs/EntityComponentSystem.h>
#include <Libs/Ecs/Component/AlphaBlendedComponent.h>
#include <Libs/Ecs/Component/CameraComponent.h>
#include <Libs/Ecs/Component/FrustumComponent.h>
#include <Libs/Ecs/Component/MeshComponent.h>
#include <Libs/Ecs/Component/NameComponent.h>
#include <Libs/Ecs/Component/SpriteComponent.h>
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
	auto& registry = _ecs.sceneRegistry();
	// TODO(Gerark) I believe the following lines should be moved to the respective System classes
	registry.on_construct<Mesh>().connect<&SceneSystem::_onMeshConstructed>(this);
	registry.on_update<Mesh>().connect<&SceneSystem::_onMeshUpdated>(this);
	registry.on_construct<CurrentCamera>().connect<&SceneSystem::_onCurrentCameraConstructed>(this);
	registry.on_construct<CurrentViewport>().connect<&SceneSystem::_onCurrentViewportConstructed>(this);
}

SceneSystem::~SceneSystem() {
}

entt::registry& SceneSystem::getRegistry() {
	return _ecs.sceneRegistry();
}

const entt::registry& SceneSystem::getRegistry() const {
	return _ecs.sceneRegistry();
}

void SceneSystem::clearScene(bool clearCameras) {
	auto& registry = _ecs.sceneRegistry();
	auto meshView = registry.view<component::Mesh>();
	registry.destroy(meshView.begin(), meshView.end());

	auto spriteView = registry.view<component::Sprite>();
	registry.destroy(spriteView.begin(), spriteView.end());

	if (clearCameras) {
		auto cameraView = registry.view<component::Camera>();
		registry.destroy(cameraView.begin(), cameraView.end());
	}
}

Entity SceneSystem::getCurrentCamera() {
	auto view = _ecs.sceneRegistry().view<component::CurrentCamera>();
	if (view.empty()) {
		throw TACTICS_EXCEPTION("No current camera entity found");
	}
	return Entity::create(*view.begin(), &_ecs.sceneRegistry());
}

void SceneSystem::_onCurrentCameraConstructed(entt::registry&, entt::entity currentCameraEntity) {
	using namespace component;
	_ecs.sceneRegistry().view<CurrentCamera>().each([this, currentCameraEntity] (auto entity) {
		if (currentCameraEntity != entity) {
			_ecs.sceneRegistry().remove<CurrentCamera>(entity);
		}
	});
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
	auto entity = Entity::create("viewport"_id, &_ecs.sceneRegistry());
	entity.addComponent<component::Viewport>(topLeft, size, clearColor);
	return entity;
}

Entity SceneSystem::createCamera(
	const HashId& name,
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
	const HashId& meshName,
	const std::vector<HashId>& materials,
	const glm::quat& rotation,
	const glm::vec3& scale
) {
	using namespace component;
	auto entity = Entity::create(""_id, &_ecs.sceneRegistry());
	auto& transform = entity.addComponent<Transform>();
	transform.setPosition(position);
	transform.setRotation(rotation);
	transform.setScale(scale);

	Mesh meshComp;
	meshComp.mesh = _resourceSystem.getResource<resource::Mesh>(meshName);
	for (auto i = 0u; i < meshComp.mesh->subMeshes.size(); ++i) {
		auto& materialName = i < materials.size() ? materials[i] : materials.back();
		auto material = _resourceSystem.getResource<resource::Material>(materialName);
		meshComp.materials.push_back(resource::Material::createInstance(material));
	}
	entity.addComponent<Mesh>(meshComp);
	return entity;
}

Entity SceneSystem::createEntity(
	const HashId& name,
	const HashId& prefabName
) {
	auto prefab = _resourceSystem.getResource<resource::Prefab>(prefabName);
	auto entity = _ecs.createEntityFromPrefab(name, prefab->entity);
	return entity;
}

Entity SceneSystem::getEntityByName(const HashId& name) {
	auto view = _ecs.sceneRegistry().view<component::Name>();
	for (auto [entity, nameComp] : view.each()) {
		if (nameComp.name == name) {
			return Entity::create(entity, &_ecs.sceneRegistry());
		}
	}

	throw TACTICS_EXCEPTION("Entity with name {} not found", name.str());
}

}
