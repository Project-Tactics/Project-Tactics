#include "EcsSystem.h"

#include "Components/MeshComponent.h"
#include "Components/TransformComponent.h"
#include "Components/AlphaBlendedComponent.h"

#include <ranges>

namespace tactics {

using namespace components;

EcsSystem::EcsSystem() {
	on_construct<Mesh>().connect<&EcsSystem::_onMeshConstructed>(this);
	on_update<Mesh>().connect<&EcsSystem::_onMeshUpdated>(this);
}

void EcsSystem::_onMeshConstructed(entt::registry& registry, entt::entity entity) {
	_updateAlphaBlended(registry, entity);
}

void EcsSystem::_onMeshUpdated(entt::registry& registry, entt::entity entity) {
	_updateAlphaBlended(registry, entity);
}

void EcsSystem::_updateAlphaBlended(entt::registry& registry, entt::entity entity) {
	auto& material = registry.get<Mesh>(entity);
	auto itr = std::ranges::find_if(material.material->getTextures(), [] (auto& pair) { return pair.second->info.useTransparency; });
	if (itr != material.material->getTextures().end()) {
		emplace<AlphaBlended>(entity);
	} else {
		remove<AlphaBlended>(entity);
	}
}

entt::entity EcsSystem::createMeshEntity(
	const glm::vec3& position,
	const glm::quat& rotation,
	const glm::vec3& scale,
	std::shared_ptr<resource::Mesh> mesh,
	std::shared_ptr<resource::Material> material) {
	auto entity = create();
	auto& transformComp = emplace<Transform>(entity);
	transformComp.setPosition(position);
	transformComp.setRotation(rotation);
	transformComp.setScale(scale);

	Mesh meshComp;
	meshComp.material = resource::Material::createInstance(material);
	meshComp.mesh = mesh;
	emplace<Mesh>(entity, meshComp);
	return entity;
}

}
