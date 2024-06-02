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

FsmAction MapState::enter() {
	_exitNextFrame = false;

	auto& sceneSystem = getService<SceneSystem>();

	// TODO(Gerark) In theory this setup of mesh with materials should be done in a resource file ( prefab/scene file? )
	auto map = sceneSystem.createEntity({}, "map1", "texturedUnlit");
	map.updateComponent<component::Mesh>([this] (auto& mapMesh) {
		for (int i = 0; i < 10; i++) {
			auto textureName = fmt::format("temp{}", i);
			mapMesh.materials[i]->set("u_Texture", getService<resource::ResourceSystem>().getResource<resource::Texture>(textureName));
		}
		mapMesh.materials[10]->set("u_Color", Color::black);
	});

	auto cameraEntity = sceneSystem.getCurrentCamera();
	cameraEntity.getComponent<component::Transform>().setPosition({0, 0, 0});
	auto& frustum = cameraEntity.getComponent<component::Frustum>();
	frustum.fov = 60;
	frustum.orthoSize = 3.f;
	cameraEntity.addComponent<component::RotateAroundPoint>(0.005f, 0.f, 30.0f, Vector3::up * 20.f, Vector3::zero);
	cameraEntity.getComponent<component::Camera>().projectionType = component::ProjectionType::Orthographic;

	return FsmAction::none();
}

FsmAction MapState::update() {
	if (_exitNextFrame) {
		return FsmAction::transition("exit");
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
}

bool MapState::onKeyPress(SDL_KeyboardEvent& event) {
	if (event.keysym.scancode == SDL_SCANCODE_ESCAPE) {
		_exitNextFrame = true;
		return true;
	}

	return false;
}

}
