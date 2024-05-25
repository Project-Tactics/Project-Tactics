#include "EcsSystem.h"

#include "Components/MaterialComponent.h"
#include "Components/MeshComponent.h"
#include "Components/TransformComponent.h"
#include "Components/AlphaBlendedComponent.h"

namespace tactics {

using namespace components;

EcsSystem::EcsSystem() {
	_registry.on_construct<Material>().connect<&EcsSystem::onMaterialConstructed>(this);
}

void EcsSystem::onMaterialConstructed(entt::registry& registry, entt::entity entity) {
	auto& material = registry.get<Material>(entity);
	if (material.texture->info.useTransparency) {
		_registry.emplace<AlphaBlended>(entity);
	}
}

entt::registry& EcsSystem::getRegistry() {
	return _registry;
}

const entt::registry& EcsSystem::getRegistry() const {
	return _registry;
}

entt::entity EcsSystem::createMeshEntity(
	const glm::vec3& position,
	const glm::quat& rotation,
	const glm::vec3& scale,
	resource::Mesh& mesh,
	resource::Shader& shader,
	resource::Texture& texture) {
	auto entity = _registry.create();
	auto& transformComp = _registry.emplace<Transform>(entity);
	transformComp.setPosition(position);
	transformComp.setRotation(rotation);
	transformComp.setScale(scale);

	auto& meshComp = _registry.emplace<Mesh>(entity);
	meshComp.mesh = &mesh;

	Material materialComp;
	materialComp.shader = &shader;
	materialComp.texture = &texture;
	_registry.emplace<Material>(entity, materialComp);
	return entity;
}

}
