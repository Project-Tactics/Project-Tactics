#include "MapState.h"

#include "../Component/RotateAroundPoint.h"

#include <Libs/Ecs/EntityComponentSystem.h>
#include <Libs/Ecs/Component/CameraComponent.h>
#include <Libs/Ecs/Component/MeshComponent.h>
#include <Libs/Ecs/Component/NameComponent.h>
#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Ecs/Component/FrustumComponent.h>
#include <Libs/Rendering/RenderSystem.h>
#include <Libs/Resource/Material/Material.h>
#include <Libs/Resource/Prefab/Prefab.h>
#include <Libs/Resource/ResourceSystem.h>
#include <Libs/Utility/Math.h>

#include <random>

namespace tactics {

MapState::MapState(ServiceLocator& serviceLocator, unsigned int mapIndex): FsmStateWithServices(serviceLocator), _mapIndex(mapIndex) {
}

FsmAction MapState::enter() {
	auto& sceneSystem = getService<SceneSystem>();

	auto mapName = fmt::format("map{:02d}", _mapIndex);
	sceneSystem.createEntity("map", mapName);

	auto cameraEntity = sceneSystem.getCurrentCamera();
	auto& frustum = cameraEntity.getComponent<component::Frustum>();
	frustum.fov = 60;
	frustum.orthoSize = 3.f;
	if (!cameraEntity.hasComponent<component::RotateAroundPoint>()) {
		cameraEntity.addComponent<component::RotateAroundPoint>(0.0035f, 0.f, 30.0f, Vector3::up * 20.f, Vector3::up * 1.f);
	}
	cameraEntity.getComponent<component::Camera>().projectionType = component::ProjectionType::Orthographic;

	return FsmAction::none();
}

FsmAction MapState::update() {
	auto& ecs = getService<EntityComponentSystem>();
	component::RotateAroundPointSystem::update(ecs.view<component::Transform, component::RotateAroundPoint>());

	return FsmAction::none();
}

void MapState::exit() {
	auto& ecsSystem = getService<EntityComponentSystem>();
	ecsSystem.view<component::Mesh>().each([&ecsSystem] (auto entity, auto&) {
		ecsSystem.destroy(entity);
	});
}

FsmEventAction MapState::onKeyPress(SDL_KeyboardEvent& event) {
	if (event.keysym.scancode == SDL_SCANCODE_ESCAPE) {
		return FsmEventAction::transition("exit");
	} else if (event.keysym.scancode == SDL_SCANCODE_RETURN) {
		return FsmEventAction::transition("next");
	} else if (event.keysym.scancode == SDL_SCANCODE_SPACE) {
		return FsmEventAction::transition("empty");
	}

	return FsmEventAction::none();
}

}
