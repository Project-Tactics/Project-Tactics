#include "MapState.h"


#include <Libs/Ecs/EntityComponentSystem.h>
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

	auto cameraEntity = sceneSystem.getCurrentCamera();
	cameraEntity.getComponent<component::Transform>().setPosition({0, 0, 0});
	cameraEntity.getComponent<component::Frustum>().fov = 60;

	auto entity = _createTerrainObject({0, 0, 0}, "dirt");
	entity.getComponent<component::Transform>().rotate(glm::radians(90.f), Vector3::left);
	/*
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(0, 2);
	std::array<std::string_view, 3> textures = {"grass", "dirt", "stone"};
	int mapHalfSizeX = 8;
	int mapHalfSizeZ = 4;
	for (int i = -mapHalfSizeX; i < mapHalfSizeX; i++) {
		for (int j = -mapHalfSizeZ; j < mapHalfSizeZ; j++) {
			_createTerrainObject({i, 0, j}, textures[distr(gen)]);
		}
	}*/

	return FsmAction::none();
}

FsmAction MapState::update() {
	if (_exitNextFrame) {
		return FsmAction::transition("exit");
	}

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

EntityObject MapState::_createCharacterObject() {
	auto& sceneSystem = getService<SceneSystem>();
	return sceneSystem.createEntity({}, "map", "texturedUnlit");
}

EntityObject MapState::_createTerrainObject(const glm::vec3& position, std::string_view textureName) {
	using namespace resource;
	using namespace component;

	auto& sceneSystem = getService<SceneSystem>();
	auto entity = sceneSystem.createEntity(position, "map", "texturedUnlit");

	auto& resourceSystem = getService<resource::ResourceSystem>();
	auto texture = resourceSystem.getResource<Texture>(textureName);

	entity.getComponent<component::Mesh>().material->set("u_Texture", texture);
	return entity;
}

}
