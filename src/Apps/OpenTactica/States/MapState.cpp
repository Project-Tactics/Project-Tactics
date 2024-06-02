#include "MapState.h"

#include "../Component/RotateAroundPoint.h"

#include <Libs/Ecs/EntityComponentSystem.h>
#include <Libs/Ecs/Component/CameraComponent.h>
#include <Libs/Ecs/Component/MeshComponent.h>
#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Ecs/Component/FrustumComponent.h>
#include <Libs/Rendering/RenderSystem.h>
#include <Libs/Resource/Material/Material.h>
#include <Libs/Resource/ResourceSystem.h>
#include <Libs/Utility/Math.h>

#include <random>

namespace tactics {

MapState::MapState(ServiceLocator& serviceLocator, unsigned int mapIndex): FsmStateWithServices(serviceLocator), _mapIndex(mapIndex) {
}

FsmAction MapState::enter() {
	_exitNextFrame = false;
	_exitNextFrameWithNextTransition = false;

	auto& sceneSystem = getService<SceneSystem>();

	// TODO(Gerark) In theory this setup of mesh with materials should be done in a resource file ( prefab/scene file? )
	auto mapName = fmt::format("map{:02d}", _mapIndex);
	auto map = sceneSystem.createEntity({}, mapName, "texturedUnlit");
	map.updateComponent<component::Mesh>([this, &mapName] (auto& mapMesh) {
		int i = 0;
		for (; i < mapMesh.mesh->subMeshes.size() - 1; i++) {
			auto textureName = fmt::format("{}_{:02d}", mapName, i);
			mapMesh.materials[i]->set("u_Texture", getService<resource::ResourceSystem>().getResource<resource::Texture>(textureName));
		}
		mapMesh.materials[i]->set("u_Color", Color::black);
	});

	auto cameraEntity = sceneSystem.getCurrentCamera();
	cameraEntity.getComponent<component::Transform>().setPosition({0, 0, 0});
	auto& frustum = cameraEntity.getComponent<component::Frustum>();
	frustum.fov = 60;
	frustum.orthoSize = 3.f;
	cameraEntity.addComponent<component::RotateAroundPoint>(0.0035f, 0.f, 30.0f, Vector3::up * 20.f, Vector3::up * 1.f);
	cameraEntity.getComponent<component::Camera>().projectionType = component::ProjectionType::Orthographic;

	return FsmAction::none();
}

FsmAction MapState::update() {
	if (_exitNextFrame) {
		_exitNextFrame = false;
		return FsmAction::transition("exit");
	} else if (_exitNextFrameWithNextTransition) {
		_exitNextFrameWithNextTransition = false;
		return FsmAction::transition("next");
	}

	auto& ecs = getService<EntityComponentSystem>();
	component::RotateAroundPointSystem::update(ecs.view<component::Transform, component::RotateAroundPoint>());

	return FsmAction::none();
}

void MapState::exit() {
	auto& ecsSystem = getService<EntityComponentSystem>();
	ecsSystem.view<component::Mesh>().each([&ecsSystem] (auto entity, auto&) {
		ecsSystem.destroy(entity);
	});

	auto& sceneSystem = getService<SceneSystem>();
	auto& cameraEntity = sceneSystem.getCurrentCamera();
	cameraEntity.removeComponent<component::RotateAroundPoint>();
}

bool MapState::onKeyPress(SDL_KeyboardEvent& event) {
	if (event.keysym.scancode == SDL_SCANCODE_ESCAPE) {
		_exitNextFrame = true;
		return true;
	} else if (event.keysym.scancode == SDL_SCANCODE_RETURN) {
		_exitNextFrameWithNextTransition = true;
		return true;
	}

	return false;
}

}
